#include <gtkmm.h>
#include "tomator_png.h"

Glib::RefPtr<Gdk::Pixbuf> get_app_icon()
{
	return Gdk::Pixbuf::create_from_inline(sizeof(tomator_png), tomator_png);
}
