# lecui tutorial

Below are step-by-step instructions on how to use the library to build a Windows gui app in C++. This file will be revised from time to time.

### Inherit from the form base class

To make a form we need to inherit from the form base class and since there is no default constructor we need to explicitly call one of the form's constructors. The one that we shall use here set's the form's caption (the text that is displayed on the top left corner of the form).

```C++
#include <liblec/lecui/form.h>
using namespace liblec;

class sample_form : public lecui::form {
public:
  sample_form() :
    form("Sample Form") {}
};
```

### Override the on_layout virtual method

The on_layout method is where we add content to the form.

```C++
#include <liblec/lecui/form.h>
using namespace liblec;

class sample_form : public lecui::form {
  bool on_layout(std::string& error) override {
    return true;
  }

public:
  sample_form() :
    form("Sample Form") {}
};
```

Things to note:
1. The on_layout method takes only one parameter, a string. If an error occurs it is written back to this string.
2. The on_layout() method returns a boolean. If the method is successful true is returned and if not false is returned (in which case error information must be written to the parameter we just mentioned).

### Add a page to the form and show it

Content can only be added to containers, and only two containers can be added directly to the form: a [page](https://github.com/alecmus/lecui/blob/master/containers/page.h) and a [status_pane](https://github.com/alecmus/lecui/blob/master/containers/status_pane.h). To add a page we need the page_management class. It is common for the page_management class to be used in various places in the class and so it is recommended that we add it as an in-class object.

```C++
#include <liblec/lecui/containers/page.h>
using namespace liblec;

class sample_form : public lecui::form {
  lecui::page_management page_man_{ *this };

  bool on_layout(std::string& error) override {
    auto& home = page_man_.add("home");
    page_man_.show("home");
    return true;
  }

public:
  sample_form() :
    form("Sample Form") {}
};
```

Things to note:
1. The add() method takes only one parameter, the alias of the page. Every page MUST have an alias and it must be unique. This is how the page is distinguished from others.
2. The add() method returns a reference to the page. We can then use this reference to add widgets to the page.
3. The show() method takes one parameter, the alias of the page to show when the form is displayed. Several pages can be created but only one can be displayed at a time.
4. Once we include any other lecui header other than <.../form.h> we no longer need <.../form.h> because it is included in every other header.

### Add a widget to the page

Widgets can be added to any container using the widget's builder class. We will add a simple label to the page using the label_builder.

```C++
#include <liblec/lecui/containers/page.h>
#include <liblec/lecui/widgets/label.h>
using namespace liblec;

class sample_form : public lecui::form {
  lecui::page_management page_man_{ *this };

  bool on_layout(std::string& error) override {
    auto& home = page_man_.add("home");

    lecui::widgets::label_builder label(home);
    label.specs()
      .text("This is a simple label")
      .rect({ 10.f, home.size().width - 10.f, 10.f, 30.f });

    page_man_.show("home");
    return true;
  }

public:
  sample_form() :
    form("Sample Form") {}
};
```

Things to note:
1. The specs() method returns a reference to the widget specifications. For more terse code we can optionally use the () operator overload as follows
```
label()
  .text(...)
  .rect(...)
```
2. The rect() method is used to specify the coordinates of the widget. We use it to specify the coordinates in this order: left, right, top, bottom. Optionally we can use chaining as follows:
```
label()
  .text(...)
  .rect().left(10.f).right(home.size().width - 10.f).top(10.f).bottom(30.f);
```

## Make an instance of the class and show the form

We can instantiate our class and call the show() method as follows to display the form:

```C++
int main() {
	sample_form form_;
	std::string error;
	if (!form_.show(error))
		form_.message(error);
	return 0;
}
```

Things to note:
1. The show() method takes only one parameter, a string. If an error occurs it is written back to this string.
2. The show() method returns a boolean. If the method is successful true is returned and if not false is returned (in which case error information will be available in the parameter we just mentioned).
3. The message() method is used to display a message box to the user. It's a simple message box with an Ok button.
