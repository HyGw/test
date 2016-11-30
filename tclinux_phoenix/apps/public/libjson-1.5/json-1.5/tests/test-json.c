/* vim:set et sw=2 cino=t0,f0,(0,{s,>2s,n-1s,^-1s,e2s: */
/* This file is part of herzi's playground
 *
 * AUTHORS
 *     Sven Herzberg
 *
 * Copyright (C) 2009  Sven Herzberg
 *
 * This work is provided "as is"; redistribution and modification
 * in whole or in part, in any medium, physical or electronic is
 * permitted without restriction.
 *
 * This work is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * In no event shall the authors or contributors be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential
 * damages (including, but not limited to, procurement of substitute
 * goods or services; loss of use, data, or profits; or business
 * interruption) however caused and on any theory of liability, whether
 * in contract, strict liability, or tort (including negligence or
 * otherwise) arising in any way out of the use of this software, even
 * if advised of the possibility of such damage.
 */

#include "json.h"

#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

static void
test_json_unescape (void)
{
  char* strings[][2] =
    {
      { /* make sure the test works */
        "simple ascii-conformant string",
        "simple ascii-conformant string"
      },
      { /* make sure we decode »\"« properly */
        "\\\"quoted string\\\"",
        "\"quoted string\""
      },
      { /* »\\« */
        "C:\\\\Windows\\\\win.com",
        "C:\\Windows\\win.com"
      },
      { /* »\/« */
        "\\/bin\\/true",
        "/bin/true"
      },
      { /* »\b« - backspace U+0008 */
        "\\b",
        "\x8"
      },
      {/* »\f« - form feed U+000C */
        "\\f",
        "\xC"
      },
      { /* »\n« - newline U+0010 */
        "\\n",
        "\n"
      },
      { /* »\r« - carriage return U+000D */
        "\\r",
        "\r"
      },
      { /* »\t« - tab U+0009 */
        "\\t",
        "\t"
      },
      { /* »\uXXXX« - UTF-8 BMP character */
        "\\u0041pfelb\\u00E4ume\\u2026",
        "Apfelb\303\244ume\342\200\246"
        /* Apfelbäume… */
      },
      { /* »\uXXXX\uXXXX« - UTF-16 surrogates */
        "\\uD835\\uDE79\\uD835\\uDE82\\uD835\\uDE7E\\uD835\\uDE7D",
        "\360\235\231\271\360\235\232\202\360\235\231\276\360\235\231\275"
        /* 𝙹𝚂𝙾𝙽 (From Unicode's "Mathematical Monospace Capitals") */ }
    };
  size_t i;

  printf ("test_json_unescape(): ");

  for (i = 0; i < (sizeof(strings)/sizeof(strings[0])); i++)
    {
      char* test = json_unescape (strings[i][0]);

      assert (test);
      if (strcmp (test, strings[i][1]))
        {
          printf ("\nerror unescaping: got »%s« (expected »%s«)\n",
                  test, strings[i][1]);
          assert (!strcmp (test, strings[i][1]));
        }

      printf (".");
      free (test);
    }

  printf ("\n");
}

static void
test_remove_items (void)
{
  char* json = "[{\"first\" : 1, \"second\" : 2}]";
  json_t* root = NULL;
  json_t* item;

  assert (JSON_OK == json_parse_document (&root, json));
  json_free_value (&root);

  assert (JSON_OK == json_parse_document (&root, json));
  assert (root->child);
  item = json_find_first_label (root->child, "first");
  json_free_value (&item);
  json_free_value (&root);

  assert (JSON_OK == json_parse_document (&root, json));
  assert (root->child);
  item = json_find_first_label (root->child, "second");
  json_free_value (&item);
  json_free_value (&root);
}

int
main (int   argc,
      char**argv)
{
  test_json_unescape ();
  test_remove_items ();

  return 0;
}
