# Airpad

## Description
Airpad is a basic, generic graphical text editor. It has all of the functionalities one would expect from such a description:

+ Opening and saving files (in many encodings)
+ Clipboard: copying, pasting and cutting text
+ Undoing and redoing of operations
+ Finding and replacing text
+ Changing the display font
+ Text wrap modes

However, it does not support printing (yet).

## Requirements
+ make
+ cc
+ gtk3
+ gettext
+ msgfmt
+ sh
+ pkgconfig
+ coreutils

GCC and Clang are guaranteed to compile the project.

## Installation
These are the steps for the basic installation, which should be appropriate in overwhelming majority of use-cases
(also make sure that you are in the project's root directory):

1. Run `make all OPTIMIZE=1`. This will build the executable with optimizations on.
1. Run `make install`. This will install the executable and all program data to appropriate places.

If you need to modify some arguments, such as the installation directories or the compiler to use, run `make help`.

**Note: If you do modify arguments, make sure that `PROG` variable is identical to `AIRPAD_EXEC_NAME` in `SWITCHES` variable.
This is because some portions of the program use `AIRPAD_EXEC_NAME` to access installed assets (which is determined by `PROG`).**

### Macros
This program can be configured during compilation time using macros, specified through `SWITCHES` make variable.  
The following table documents those macros.

| **Name**            | **Type** | **Default**               | **Description**                                                                                                                                |
|---------------------|----------|---------------------------|------------------------------------------------------------------------------------------------------------------------------------------------|
| AIRPAD\_EXEC\_NAME  | string   |                           | Name used in the program assets and of the program itself. It is recommended to set it through `PROG` variable rather than through `SWITCHES`. |
| AIRPAD\_INFO\_IDEN  | string   | "com.gitlab.NH000.Airpad" | Application identifier. It is recommended to set it through `IDEN` variable rather than through  `SWITCHES`.                                   |
| AIRPAD\_LOCALE\_DIR | string   |                           | Directory where MO localization files are located. It is recommended to set it through `LOCALEDIR` variable rather than through `SWITCHES`.    |

**Note: Some variables not having the default value in the table above does not mean that they need to be defined by user necessarily.
All that means is that they do not have default values specified in the program source, but all variables have sane defaults specified
using `SWITCHES` variable in `Makefile`.**

**Note: If you do use `SWITCHES`, you should not prefix each variable with "-D", as that is already done by the makefile.**

## Uninstallation
Just run `make uninstall` in the project's root directory to uninstall the program and all its assets (but the configuration file will not be removed).  
Same as for the installation, you can specify parameters (e.g. location of the installed program, etc.) through make
variables (run `make help` to see them). They all have the same meaning as for the installation.

## Translations
Default language of this program (in effect when `C` or `POSIX` is set as locale) is US English.  
The following translations are available:

| **Language**       | **Translator**                                           | **For versions** |
|--------------------|----------------------------------------------------------|------------------|
| Serbian (Cyrillic) | [Nikola Hadžić](mailto:nikola.hadzic.000@protonmail.com) | 1.0              |
| Serbian (Latin)    | [Nikola Hadžić](mailto:nikola.hadzic.000@protonmail.com) | 1.0              |

### Translation process
This program is written to be easily translatable into multiple languages, and it achieves that through the use of [`gettext`](https://www.gnu.org/software/gettext/) library.
Translations are located in `po` directory. Files in that directory contain translations, each PO file corresponding to one locale.

To add a new or update existing translation, enter the project's `po` directory and run the following command:

```
make %.po  # Generate/update PO file; "%" should be replaced with a language code.
```

Afterwards, you can edit created/updated PO file (see [`gettext` manual](https://www.gnu.org/software/gettext/manual/gettext.html) for details),
translating the program that way.

You could also run `make messages.pot` to just generate the template file, but this will be done automatically by the previously described rule.

In PO files you should find untranslated line `msgid "translator-credits"`: as translation write your own name (with optionally your email/website) there.

Also, it is recommended to add/update translations of `.desktop` file entry as well.
Desktop file is generated by `Makefile` script, so you'll find its contents there.
