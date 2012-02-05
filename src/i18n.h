
#ifndef BALLER_I18N_H
#define BALLER_I18N_H

#include <config.h>

#if HAVE_GETTEXT

#include <libintl.h>
#define _(string) gettext(string)
#define gettext_noop(string) string
#define N_(string) gettext_noop(string)

#else

#define _(string) (string)
#define N_(string) string
#define textdomain(domain)
#define bindtextdomain(package, directory)

#endif  /* HAVE_GETTEXT */

#endif  /* BALLER_I18N_H */
