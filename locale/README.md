
## [A Quick Gettext Tutorial](https://www.labri.fr/perso/fleury/posts/programming/a-quick-gettext-tutorial.html)

Recently I had to look for tutorials about Gettext to explain it to some of my students. I realized that most of the tutorials pointed by Google were just wrong or too messy to be understood properly. Most of the proper documentation can be found in the official  [GNU  Gettext documentation](https://www.gnu.org/software/gettext/manual/gettext.html). But, here is a short summary of the important points to start with.

### Gettext in Few Words

First, Gettext is a library designed to minimize the amount of work to put into the translation of end-user messages within the code. It handles both  **internationalization**  and  **localization**.

**_Note_**: internationalization and localization are such long words that most of the people will replace it by i18n (first-letter, number of letters, last-letter) and l10n (same as previous). So, do not be surprised to find  `i18n`  or  `l10n`  everywhere in documentation or source code, there are just short-ends to write long words.

**Internationalization**  refer to the ability, for a program, to support user-messages translated into several languages or dialects. And,  **localization**  refer to the process of specializing a whole computer environment for a language or a culture, usually through the usage of shell environment variables.

For example,  UNIX  platforms usually rely on the environment variables  `LC_ALL`,  `LC_xxx`,  `LANGUAGE`  and  `LANG`  to set-up the localization of the user environment. These variables are mostly the ones read by the Gettext framework to select which language to use (with the default being ‘English’).

### The Full Gettext Process

Gettext works by, first, generating a template file with all the strings to be translated directly extracted from the source files, this template file is called a  `.pot`  file which stands for  **Portable Object Template**. Once you get this template file, you can use it to create one file per language supported by your software, these new files are called  `.po`  files which stands for  **Portable Object**. Then, in order to make it machine readable, these  `.po`  files are translated to  `.mo`  files which stands for  **Machine Object** files.

The full process starting from the source file  `.c`  to the  `.mo`  file is the following (along with the tools used to perform the parsing and the rewriting):
```bash
         xgettext                 msginit                   msgfmt
hello.c ----------> po/hello.pot ---------> po/fr/hello.po --------> po/fr/hello.mo
```
If a new string has been added to the source code, then the process of merging changes should follow this process:
```bash
xgettext -j                  msgmerge                   msgfmt
hello.c -------------> po/hello.pot -----------> po/fr/hello.po --------> po/fr/hello.mo
```

### The ‘Hello World!’ Example

Lets start with a minimalist ‘Hello World’ example, here is the source code:
```c++
#include <stdio.h>
#include <stdlib.h>

#include <libintl.h>
#include <locale.h>

#define _(STRING) gettext(STRING)

int main()
{
  /* Setting the i18n environment */
  setlocale (LC_ALL, "");
  bindtextdomain ("hello", "/usr/share/locale/");
  textdomain ("hello");

  /* Example of i18n usage */
  printf(_("Hello World\n"));

  return EXIT_SUCCESS;
}
```
You should note that we added a macro to shorten the  `gettext(...)`  function into  `_(...)`. This is an extremely common idiom in matter of internationalization. Try to get used to it, you will see it very often in usual code.

We aim to have the following structure for our project:
```
hello/
 |-- hello.c
 |-- Makefile
 \-- po/
 |--hello.pot
 \-- fr/
 |-- hello.mo
 \-- hello.po
```
Lets first suppose that we only have:
```
hello/
 |-- hello.c
 |-- Makefile
 \-- po/
 \-- fr/
```
Lets create the  `hello.pot`  file (we are located in the  `hello/`  directory):
```bash
$> xgettext --keyword=_ --language=C --add-comments --sort-output -o po/hello.pot hello.c
```
A  `po/hello.pot`  template is created and should look like this:
```
#  SOME  DESCRIPTIVE  TITLE.
#  Copyright  (C)  YEAR  THE  PACKAGE'S  COPYRIGHT  HOLDER
#  This  file  is  distributed  under  the  same  license  as  the  PACKAGE  package.
#  FIRST  AUTHOR  <EMAIL@ADDRESS>,  YEAR.
#
#,  fuzzy
msgid  ""
msgstr  ""
"Project-Id-Version: PACKAGE VERSION\n"
"Report-Msgid-Bugs-To: \n"
"POT-Creation-Date: 2016-01-25 18:51+0100\n"
"PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\n"
"Last-Translator: FULL NAME <EMAIL@ADDRESS>\n"
"Language-Team: LANGUAGE <LL@li.org>\n"
"Language: \n"
"MIME-Version: 1.0\n"
"Content-Type: text/plain; charset=CHARSET\n"
"Content-Transfer-Encoding: 8bit\n"

#:  hello.c:11
#,  c-format
msgid  "Hello World\n"
msgstr  ""
```
Let customize a bit this template by replacing a few default by proper values (`Project-Id-Version`,  `Report-Msgid-Bugs-To`  and  `charset`):
```
#  SOME  DESCRIPTIVE  TITLE.
#  Copyright  (C)  YEAR  THE  PACKAGE'S  COPYRIGHT  HOLDER
#  This  file  is  distributed  under  the  same  license  as  the  PACKAGE  package.
#  FIRST  AUTHOR  <EMAIL@ADDRESS>,  YEAR.
#
#,  fuzzy
msgid  ""
msgstr  ""
"Project-Id-Version: Hello 1.0\n"
"Report-Msgid-Bugs-To: andrea@nonymous.net\n"
"POT-Creation-Date: 2016-01-25 18:51+0100\n"
"PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\n"
"Last-Translator: FULL NAME <EMAIL@ADDRESS>\n"
"Language-Team: LANGUAGE <LL@li.org>\n"
"Language: \n"
"MIME-Version: 1.0\n"
"Content-Type: text/plain; charset=UTF-8\n"
"Content-Transfer-Encoding: 8bit\n"

#:  hello.c:11
#,  c-format
msgid  "Hello World\n"
msgstr  ""
```
So, now, we have full template to start with, we can start to work on a translation for this program. Lets start with a French translation of it. We need to create a file  `po/fr/hello.po`  extracted from the template.
```bash
$> msginit --input=po/hello.pot --locale=fr --output=po/fr/hello.po
```
A new  `po/fr/hello.po`  file just appeared:
```
#  French  translations  for  Hello  package.
#  Copyright  (C)  2016  THE  Hello'S  COPYRIGHT  HOLDER
#  This  file  is  distributed  under  the  same  license  as  the  Hello  package.
#  Andrea  Nonymous  <andrea@nonymous.net>,  2016.
#
msgid  ""
msgstr  ""
"Project-Id-Version: Hello 1.0\n"
"Report-Msgid-Bugs-To: andrea@nonymous.net \n"
"POT-Creation-Date: 2016-01-25 18:51+0100\n"
"PO-Revision-Date: 2016-01-25 19:53+0100\n"
"Last-Translator: Andrea Nonymous <andrea@nonymous.net>\n"
"Language-Team: French\n"
"Language: fr\n"
"MIME-Version: 1.0\n"
"Content-Type: text/plain; charset=UTF-8\n"
"Content-Transfer-Encoding: 8bit\n"
"Plural-Forms: nplurals=2; plural=(n > 1);\n"

#:  hello.c:12
#,  c-format
msgid  "Hello World\n"
msgstr  ""
```
Now, we can translate the only string in the program:
```
#: hello.c:12
#, c-format
msgid "Hello World\n"
msgstr "Bonjour le monde\n"
```
The last step, before installing the software, is to generate the  `.mo`  file:
```bash
$> msgfmt --output-file=po/fr/hello.mo po/fr/hello.po
```
And this is it!
### Testing our HelloWorld Program
Lets test if the program is working properly. We first have to modify slightly the C code in order to point somewhere else than  `/usr/share/locale`  and point to our source directory. We use a  `getenv("PWD")`  to locate our current position:
```c++
#include <stdio.h>
#include <stdlib.h>

#include <libintl.h>
#include <locale.h>

#define _(STRING) gettext(STRING)

int main()
{
  /* Setting the i18n environment */
  setlocale (LC_ALL, "");
  bindtextdomain ("hello", getenv("PWD"));
  textdomain ("hello");

  /* Example of i18n usage */
  printf(_("Hello World\n"));

  return EXIT_SUCCESS;
}
```
Then, we need to install the  `po/fr/hello.mo`  file to the place searched by the system and the right place to go is  `./fr/LC_MESSAGES/hello.mo`.

Finally, we can try it:
```bash
$> ./hello 
Hello World
$> LANG=fr_FR ./hello
Bonjour le monde
$>
```
### Updating the  PO/POT Files

From time to time, you might add a few strings into the original source files. If you do, you do not need to restart everything from scratch, you may simply merge what you have with the new strings. For example, lets assume that we modified our Hello package sources as follow:
```c++
#include <stdio.h>
#include <stdlib.h>

#include <libintl.h>
#include <locale.h>

#define _(STRING) gettext(STRING)

int main()
{
  /* Setting the i18n environment */
  setlocale (LC_ALL, "");
  bindtextdomain ("hello", "/usr/share/locale/");
  textdomain ("hello");

  printf(_("Hello World\n"));
  puts(_("Good night"));

  return EXIT_SUCCESS;
}
```

We first need to update the  `.pot`  file (as previously):

`$> xgettext --keyword=_ --language=C --add-comments --sort-output -o po/hello.pot hello.c` 

Then, we merge it with the previous  `po/fr/hello.po`  file :

`$> msgmerge --update po/fr/hello.po po/hello.pot` 

Finally, we write the translation of the new string in the  `.po`  file and build the  `.mo`  file as follow:

`$> msgfmt --output-file=po/fr/hello.mo po/fr/hello.po` 

And, we are done…

Just to finish it up, here is the Makefile that I used:
```bash
CFLAGS = -Wall -Wextra -std=c99 -g
CPPFLAGS = -I.
LDFLAGS =

EXEC = hello

all: $(EXEC) po/fr/$(EXEC).mo

$(EXEC): $(EXEC).c 
    $(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LDFLAGS)

po/fr/$(EXEC).mo: po/fr/$(EXEC).po
    msgfmt --output-file=$@ $<

po/fr/$(EXEC).po: po/$(EXEC).pot
    msgmerge --update $@ $<

po/$(EXEC).pot: $(EXEC).c
    xgettext -k_ -j -lC -c -s -o po/hello.pot hello.c

clean:
    @rm -f $(EXEC) po/fr/*.mo *~

.PHONY: clean` 
```
### Conclusion
Gettext is a quite efficient and simple way to perform internationalization of software once it is up and running. The only difficulty is just to initialize the process. I hope that this short tutorial made it clear. But, still if you want to know more about all the process, feel free to read the  [GNU  Gettext documentation](https://www.gnu.org/software/gettext/manual/gettext.html).
