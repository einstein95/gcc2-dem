#include "demangle.h"
#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *program_name;
static char *program_version = "2.95";
static int flags = DMGL_PARAMS | DMGL_ANSI;

static void demangle_it PARAMS((char *));
static void usage PARAMS((FILE *, int));

void fatal(char *str) {
  fprintf(stderr, "%s: %s\n", program_name, str);
  exit(1);
}

static void demangle_it(char *mangled_name) {
  char *result;

  result = cplus_demangle(mangled_name, flags);
  if (result == NULL) {
    printf("%s\n", mangled_name);
  } else {
    printf("%s\n", result);
    free(result);
  }
}

static void usage(FILE *stream, int status) {
  fprintf(stream, "\
Usage: %s [-_] [-n] [-s {gnu,lucid,arm,hp,edg}] [--strip-underscores]\n\
       [--no-strip-underscores] [--format={gnu,lucid,arm,hp,edg}]\n\
      [--help] [--version] [arg...]\n",
          program_name);
  exit(status);
}

#define MBUF_SIZE 32767
char mbuffer[MBUF_SIZE];

/* Defined in the automatically-generated underscore.c.  */
// extern int prepends_underscore;

int strip_underscore = 0;

static struct option long_options[] = {
    {"strip-underscores", no_argument, 0, '_'},
    {"format", required_argument, 0, 's'},
    {"help", no_argument, 0, 'h'},
    {"java", no_argument, 0, 'j'},
    {"no-strip-underscores", no_argument, 0, 'n'},
    {"version", no_argument, 0, 'v'},
    {0, no_argument, 0, 0}};

/* More 'friendly' abort that prints the line and file.
   config.h can #define abort fancy_abort if you like that sort of thing.  */

void fancy_abort() { fatal("Internal gcc abort."); }

int main(int argc, char **argv) {
  char *result;
  int c;

  program_name = argv[0];

  strip_underscore = 0;

  while ((c = getopt_long(argc, argv, "_ns:j", long_options, (int *)0)) !=
         EOF) {
    switch (c) {
    case '?':
      usage(stderr, 1);
      break;
    case 'h':
      usage(stdout, 0);
    case 'n':
      strip_underscore = 0;
      break;
    case 'v':
      printf("GNU %s (C++ demangler), version %s\n", program_name,
             program_version);
      exit(0);
    case '_':
      strip_underscore = 1;
      break;
    case 'j':
      flags |= DMGL_JAVA;
      break;
    case 's':
      if (strcmp(optarg, "gnu") == 0) {
        current_demangling_style = gnu_demangling;
      } else if (strcmp(optarg, "lucid") == 0) {
        current_demangling_style = lucid_demangling;
      } else if (strcmp(optarg, "arm") == 0) {
        current_demangling_style = arm_demangling;
      } else if (strcmp(optarg, "hp") == 0) {
        current_demangling_style = hp_demangling;
      } else if (strcmp(optarg, "edg") == 0) {
        current_demangling_style = edg_demangling;
      } else {
        fprintf(stderr, "%s: unknown demangling style `%s'\n", program_name,
                optarg);
        exit(1);
      }
      break;
    }
  }

  if (optind < argc) {
    for (; optind < argc; optind++) {
      demangle_it(argv[optind]);
    }
  } else {
    for (;;) {
      int i = 0;
      c = getchar();
      /* Try to read a label.  */
      while (c != EOF &&
             (isalnum(c) || c == '_' || c == '$' || c == '.' || c == '<' ||
              c == '>' || c == '#' || c == ',' || c == '*' || c == '&' ||
              c == '[' || c == ']' || c == ':' || c == '(' || c == ')'))
      /* the ones in the 2nd & 3rd lines were added to handle
         HP aCC template specialization manglings */
      {
        if (i >= MBUF_SIZE - 1)
          break;
        mbuffer[i++] = c;
        c = getchar();
      }
      if (i > 0) {
        int skip_first = 0;

        if (mbuffer[0] == '.')
          ++skip_first;
        if (strip_underscore && mbuffer[skip_first] == '_')
          ++skip_first;

        if (skip_first > i)
          skip_first = i;

        mbuffer[i] = 0;

        result = cplus_demangle(mbuffer + skip_first, flags);
        if (result) {
          if (mbuffer[0] == '.')
            putc('.', stdout);
          fputs(result, stdout);
          free(result);
        } else
          fputs(mbuffer, stdout);

        fflush(stdout);
      }
      if (c == EOF)
        break;
      putchar(c);
    }
  }

  exit(0);
}