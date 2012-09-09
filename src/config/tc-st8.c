#include <stdio.h>
#include <stdbool.h>
#include "tc-st8.h"
#include "../as.h"

struct stm8_opcodes_s
{
  char *        name;
  stm8_arg_t    constraints[5];
  unsigned int  bin_opcode;
};

struct stm8_opcodes_s stm8_opcodes[] =
{
  #include "opcode/stm8.h"
  {NULL, {ST8_END}, 0},
};

static struct hash_control *stm8_hash;

const char comment_chars[] = ";";
const char line_comment_chars[] = "#";
const char line_separator_chars[] = "{";

const char md_short_jump_size = 3;
const char md_long_jump_size = 4;

/* The target specific pseudo-ops which we support.  */
/* example:
const pseudo_typeS md_pseudo_table[] =
{
  {"arch", avr_set_arch,	0},
  { NULL,	NULL,		0}
};
*/
const pseudo_typeS md_pseudo_table[] =
{
  { NULL,	NULL,		0}
};

const char EXP_CHARS[] = "eE";

/* Chars that mean this number is a floating point constant.
   As in 0f12.456
   or    0d1.2345e12  */
const char FLT_CHARS[] = "rRsSfFdDxXpP";

/* On the Z8000, a PC-relative offset is relative to the address of the
   instruction plus its size.  */
long
md_pcrel_from (fixS *fixP)
{
  return fixP->fx_size + fixP->fx_where + fixP->fx_frag->fr_address;
}

char *
md_atof (int type, char *litP, int *sizeP)
{
  return ieee_md_atof (type, litP, sizeP, TRUE);
}

void
md_show_usage (FILE *stream)
{
  fprintf (stream, _("\
  STM8 options:\n\
  "));
}

const char *md_shortopts = "";

struct option md_longopts[] =
  {
    {NULL, no_argument, NULL, 0}
  };

size_t md_longopts_size = sizeof (md_longopts);

arelent*
tc_gen_reloc (asection *section ATTRIBUTE_UNUSED,
	      fixS *fixp      ATTRIBUTE_UNUSED)
{
	as_bad_where (fixp->fx_file, fixp->fx_line,
                    _("stm8: don't know how to reloc"));
}

void
md_begin (void)
{
	// as_bad(_("begin"));
	struct stm8_opcodes_s *opcode;
	stm8_hash = hash_new ();
	
	/* Insert unique names into hash table.  This hash table then provides a
	quick index to the first opcode with a particular name in the opcode
	table.  */
	for (opcode = stm8_opcodes; opcode->name; opcode++)
		hash_insert (stm8_hash, opcode->name, (char *) opcode);
	/* test */
	/*
	char *frag = frag_more (6);
	bfd_putl16(0x50, frag);
	frag = frag_more (6);
	bfd_putl16(0x60, frag);
	//bfd_putl16(0x60, frag);
	*/
}

static inline char *
skip_space (char *s)
{
  while (*s == ' ' || *s == '\t')
    ++s;
  return s;
}

/* Extract one word from FROM and copy it to TO.  */

static char *
extract_word (char *from, char *to, int limit)
{
  char *op_end;
  int size = 0;

  /* Drop leading whitespace.  */
  from = skip_space (from);
  *to = 0;

  /* Find the op code end.  */
  for (op_end = from; *op_end != 0 && is_part_of_name (*op_end);)
    {
      to[size++] = *op_end++;
      if (size + 1 >= limit)
	break;
    }

  to[size] = 0;
  return op_end;
}

void
md_operand (expressionS * exp)
{
  /* In case of a syntax error, escape back to try next syntax combo.  */
  as_bad(_("stm8: call to md_operand"));
  //if (exp->X_op == O_absent)
  //  gas_cgen_md_operand (exp);
}


/* Attempt to simplify or eliminate a fixup. To indicate that a fixup
   has been eliminated, set fix->fx_done. If fix->fx_addsy is non-NULL,
   we will have to generate a reloc entry.  */
void
md_apply_fix (fixS *fixp, valueT *valp, segT segment ATTRIBUTE_UNUSED) {
	if(fixp->fx_label) {
		const char *label_name = S_GET_NAME(fixp->fx_label);
		symbolS *label = symbol_find(label_name);
		if(!label || !S_IS_DEFINED(label)) {
			as_bad(_("Unknown label %s at line %d\n"), label_name, fixp->fx_line);
			return;
		}
		unsigned int location = 0x8000 + S_GET_VALUE(label);
		unsigned char *where = fixp->fx_frag->fr_literal + fixp->fx_where;
		bfd_put_bits(location, where, 24, true);
		fixp->fx_done = 1;
	} else {
		as_bad_where (fixp->fx_file, fixp->fx_line,
                    _("stm8: don't know how to apply fix"));
	}
}

valueT
md_section_align (segT seg, valueT size)
{
  int align = bfd_get_section_alignment (stdoutput, seg);
  valueT mask = ((valueT) 1 << align) - 1;

  return (size + mask) & ~mask;
}

int
md_parse_option (int c, char *arg)
{
	return 1;
}

symbolS *
md_undefined_symbol (char *name ATTRIBUTE_UNUSED)
{
  return 0;
}


void
md_create_long_jump (char *ptr,
		     addressT from_addr ATTRIBUTE_UNUSED,
		     addressT to_addr,
		     fragS *frag,
		     symbolS *to_symbol)
{
	as_bad(_("long_jump"));
}

void
md_create_short_jump (char *ptr,
		      addressT from_addr,
		      addressT to_addr ATTRIBUTE_UNUSED,
		      fragS *frag ATTRIBUTE_UNUSED,
		      symbolS *to_symbol ATTRIBUTE_UNUSED)
{
	as_bad(_("short_jump"));
}

/* Put number into target byte order.  */

void
md_number_to_chars (char *ptr, valueT use, int nbytes)
{
  number_to_chars_bigendian (ptr, use, nbytes);
}

int
md_estimate_size_before_relax (fragS *fragP ATTRIBUTE_UNUSED,
                               segT segment_type ATTRIBUTE_UNUSED)
{
  printf (_("call to md_estimate_size_before_relax\n"));
  abort ();
}

void
md_convert_frag (bfd *abfd ATTRIBUTE_UNUSED,
                 segT sec ATTRIBUTE_UNUSED,
                 fragS *fragP ATTRIBUTE_UNUSED)
{
  printf (_("call to md_convert_frag\n"));
  abort ();
}

int split_words(char *str, char **chunks) {
	int i, max = 2;
	const char delim[] = ", ";
	const char *trail;
	memset(chunks, max, 0);
	for(i = 0; i < max; i++) {
		chunks[i] = strtok(str, delim);
		str = NULL;
		if(!chunks[i])
			break;
	}
	if(trail = strtok(NULL, delim))
		as_bad(_("Trailing characters: %s%s"), delim, trail);
	return(i);
}

int getnumber(const char *str, int *out) {
	int i;
	for(i = 0; i < strlen(str); i++) {
		if(!isdigit(str[i]))
			return 0;
	}
	*out = atoi(str);
	return 1;
}

int gethex(const char *str, int *out) {
	int i;
	for(i = 0; i <  strlen(str); i++) {
		if(!isdigit(str[i]) &&
		!(toupper(str[i]) >= 'A' && toupper(str[i]) <= 'F'))
			return 0;
	}
	sscanf(str, "%x", out);
	return 1;
}

expressionS last_exp;
/* In: argument
   Out: value
   Modifies: type */
int read_arg(char *str, stm8_arg_t *type) {
	/* There is a number of addressing modes in ST8 architecture.
	   We need to properly handle each of them in order to find a proper opcode. */
	#define RETURN(x) (x); return value;
	int value;
	if(!str) return(ST8_END);
	int length = strlen(str);
	/* direct bytes */
	if(getnumber(str, &value)) { RETURN(*type = ST8_BYTE); }

	/* absolute address */
	if(!memcmp(str, "0x", 2) || !memcmp(str, "#$", 2)) {
		if(length-2 <= 2) *type = ST8_BYTE;
		if(length-2 >= 3) *type = ST8_WORD;
		if(gethex(str+2, &value))
			return(value);
	}

	if(str[0] == '$') {
		if(length-1 <= 2) *type = ST8_SHORTMEM;
		if(length-1 >= 3) *type = ST8_LONGMEM;
		if(length-1 >= 5) *type = ST8_EXTMEM;
		if(gethex(str+1, &value))
			return(value);
	}

	/* registers */
	if(!strcmp(str, "A")) { RETURN(*type = ST8_REG_A); }
	if(!strcmp(str, "X")) { RETURN(*type = ST8_REG_X); }
	if(!strcmp(str, "Y")) { RETURN(*type = ST8_REG_Y); }

	/* relative address */
	input_line_pointer = str;
	expression(&last_exp);
	if(last_exp.X_add_symbol) {
		char *name = S_GET_NAME(last_exp.X_add_symbol);
		value = last_exp.X_add_number;
		if(!strcmp(name, "SP")) { RETURN(*type = ST8_SPREL); }
		if(!strcmp(name, "PC")) { RETURN(*type = ST8_PCREL); }
		if(last_exp.X_op == O_symbol) { RETURN(*type = ST8_SYMBOL); }
	}

	/* Can't parse an expression, notifying caller about that. */
	*type = ST8_END;
	return(0);
}

unsigned int bytes_count(unsigned int number) {
	int i;
	for(i = sizeof(int); i > 0; i--) {
		if(number & 0xFF << (i-1)*8)
			return(i);
	}
	return(1);
}

int compute_insn_size(struct stm8_opcodes_s opcode) {
	int i, ret = 0;
	for(i = 0; opcode.constraints[i] != ST8_END; i++) {
		switch(opcode.constraints[i]) {
			case ST8_SPREL:
			case ST8_SHORTMEM:
			case ST8_BYTE:
				ret++;
				break;
			case ST8_LONGMEM:
			case ST8_WORD:
				ret += 2;
				break;
			case ST8_EXTMEM:
				ret += 3;
				break;
		}
	}
	ret += bytes_count(opcode.bin_opcode);
	return(ret);
}

int read_args(char *str, stm8_arg_t *types, int *values) {
	memset(types, 0, sizeof(stm8_arg_t) * 2);
	memset(values, 0, sizeof(int) * 2);
	char *chunks[2];
	int count = split_words(str, chunks);
	int i;
	for(i = 0; i < count; i++) {
		int ret = read_arg(chunks[i], &(types[i]));
		if(types[i] == ST8_END) as_bad("Invalid operand: %s", chunks[i]);
		values[i] = ret;
	}
	return(count);
}

void stm8_bfd_out(stm8_arg_t *spec, int *values, int count, char *frag) {
	int i;
	int where = frag - frag_now->fr_literal;
	for(i = 0; i < count; i++) {
		frag++;
		switch(spec[i]) {
			case ST8_SYMBOL:
				fix_new_exp(frag_now, where, 3, &last_exp, FALSE, BFD_RELOC_32);
				break;
			case ST8_SPREL:
			case ST8_SHORTMEM:
			case ST8_BYTE:
				bfd_put_bits(values[i], frag, 8, true);
				break;
			case ST8_LONGMEM:
			case ST8_WORD:
				bfd_putb16(values[i], frag);
				break;
			case ST8_EXTMEM:
				bfd_put_bits(values[i], frag, 24, true);
				break;
			case ST8_REG_A:
			case ST8_REG_X:
			case ST8_REG_Y:
				frag--;
				/* Don't need to output anything */
				break;
		}
	}
}

int cmpspec(stm8_arg_t *spec_a, stm8_arg_t *spec_b, int count) {
	int i, ret = 0;
	for(i = 0; i < count; i++) {
		if(spec_a[i] == ST8_SYMBOL || spec_b[i] == ST8_SYMBOL)
			continue; // ST8_SYMBOL matches any
		if(!spec_a || !spec_b)
			continue; // End
		if(spec_a[i] != spec_b[i]) ret++;
	}
	return(ret);
}

/* This is the guts of the machine-dependent assembler.  STR points to a
   machine dependent instruction.  This function is supposed to emit
   the frags/bytes it assembles to.  */

void
md_assemble (char *str)
{
	char op[11];
	char *t = input_line_pointer;
	char *str_orig = strdup(str);
	str = skip_space (extract_word (str, op, sizeof (op)));
	stm8_arg_t spec[2];
	int values[2];
	int count = read_args(str, spec, values);
	struct stm8_opcodes_s *opcode = (struct stm8_opcodes_s *) hash_find (stm8_hash, op);

	if (opcode == NULL) {
		as_bad (_("unknown opcode `%s'"), op);
		return;
	}

	int i;
	for(i = 0; opcode[i].name != NULL; i++) {
		if(!cmpspec(opcode[i].constraints, spec, count)) {
			int insn_size = compute_insn_size(opcode[i]);
			char *frag = frag_more(insn_size);
			int opcode_length = bytes_count(opcode[i].bin_opcode);
			bfd_put_bits(opcode[i].bin_opcode, frag, opcode_length * 8, true);
			stm8_bfd_out(spec, values, count, frag);
			break;
		}
	}
	if(!opcode[i].name)
		as_bad("Invalid instruction: %s", str_orig);
	input_line_pointer = t;
	//free(str_orig);
}
