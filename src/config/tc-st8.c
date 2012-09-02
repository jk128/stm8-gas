#include <stdio.h>
#include "tc-st8.h"
#include "../as.h"

struct stm8_opcodes_s
{
  char *        name;
  stm8_arg_t    constraints[5];
  int           insn_size;		/* In words.  */
  unsigned int  bin_opcode;
};

struct stm8_opcodes_s stm8_opcodes[] =
{
  #include "opcode/stm8.h"
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
	as_bad_where (fixp->fx_file, fixp->fx_line,
                    _("stm8: don't know how to apply fix"));
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

typedef struct {
	stm8_arg_t type;
	unsigned int value;
} stm8_tokens_t;

int split_words(char *str, char **chunks) {
	int ret = 0;
	const char delim[] = ", ";
	const char *trail;
	chunks[0] = strtok(str, delim);
	ret++;
	if(chunks[0]) chunks[1] = strtok(NULL, delim);
	if(chunks[1]) ret++;
	if(trail = strtok(NULL, delim))
		fprintf(stderr, "Trailing characters: %s%s", delim, trail);
	return(ret);
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
		!(toupper(str[i]) >= 'a' && toupper(str[i]) <= 'b'))
			return 0;
	}
	sscanf(str, "%x", out);
	return 1;
}

/* In: argument
   Out: 1 on success
   Modifies: ret */
int tokenize_arg(char *str, stm8_tokens_t *ret) {
	/* There is a number of addressing modes in ST8 architecture.
	   We need to properly handle each of them in order to find a proper opcode. */
	#define RETURN(x) (x); return 1;
	int length = strlen(str);
	if(getnumber(str, &ret->value)) { RETURN(ret->type = ST8_BYTE); }

	if(memcmp(str, "0x", 2) || memcmp(str, "#$", 2)) {
		if(length <= 2)	ret->type = ST8_BYTE;
		if(length >= 3)	ret->type = ST8_WORD;
		if(gethex(str+2, &ret->value))
			return(1);
	}

	if(str[0] == '$') {
		if(length <= 2)	ret->type = ST8_SHORTMEM;
		if(length >= 3)	ret->type = ST8_LONGMEM;
		if(length >= 5)	ret->type = ST8_EXTMEM;
		if(gethex(str+1, &ret->value))
			return(1);
	}
	return(0);
}

int tokenize_args(char *str, stm8_tokens_t *ptr) {
	char *chunks[2];
	int count = split_words(str, chunks);
	int i;
	for(i = 0; i < count; i++) {
		int ret = tokenize_arg(chunks[i], &(ptr[i]));
		if(!ret) as_bad("Couldn't parse chunk: %s", chunks[i]);
	}
	return(count);
}

struct stm8_opcodes_s *find_opcode(const char *name, stm8_tokens_t *tokens) {
	return(hash_find(stm8_hash, name));
}

/* This is the guts of the machine-dependent assembler.  STR points to a
   machine dependent instruction.  This function is supposed to emit
   the frags/bytes it assembles to.  */

void
md_assemble (char *str)
{
char op[11];

	struct stm8_opcodes_s *opcode;
	str = skip_space (extract_word (str, op, sizeof (op)));
	as_warn("op=%s str=%s\n", op, str);
	stm8_tokens_t tokens[2];
	tokenize_args(str, tokens);
	return;
	opcode = (struct stm8_opcodes_s *) hash_find (stm8_hash, op);

	if (opcode == NULL)
	{
		as_bad (_("unknown opcode `%s'"), op);
		return;
	}

	unsigned int data;
	char *frag = frag_more(opcode->insn_size);
	bfd_putl16(opcode->bin_opcode, frag);
}
