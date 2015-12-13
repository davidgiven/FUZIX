/*
 *	Generate the syscall functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syscall_name.h"

static char namebuf[128];

static void write_call(int n)
{
  FILE *fp;
  snprintf(namebuf, 128, "fuzixarm/syscall_%s.s", syscall_name[n]);
  fp = fopen(namebuf, "w");
  if (fp == NULL) {
    perror(namebuf);
    exit(1);
  }
  fprintf(fp, "\t.text\n\n"
	      "\t.globl %1$s\n\n"
	      "%1$s:\n", syscall_name[n]);

  if (syscall_args[n] == VARARGS)
  {
	/* Varargs syscalls have the first argument in r0 and the others on
	 * the stack. We support up to four parameters. */
	fprintf(fp, "\tldmia sp, {r1, r2, r3}\n");
  }

  /* On entry, the four parameters are in r0-r3. The syscall number is
   * in r4. */

  fprintf(fp, "\tmov r4, #%d\n"
              "\tb _syscall\n",
			  n);
  fclose(fp);
}

static void write_call_table(void)
{
  int i;
  for (i = 0; i < NR_SYSCALL; i++)
    write_call(i);
}

int main(int argc, char *argv[])
{
  write_call_table();
  exit(0);
}
