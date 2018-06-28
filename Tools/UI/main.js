var id_num = 0;

var selected = null;

var guis = []

function newId()
{
	return id_num++;
}

function gui_options()
{

}

function new_gui()
{
    var id = "gui" + newId();
	var $gui = $ ("<div class='kgm_base kgm_gui'>gui</div>");
	$("#palette").append($gui);
    $gui.draggable();
    $gui.onclick(function(){
        selected = $gui;
        gui_options($gui);
    });

    var g = { target: $gui,
            };

    guis.pushBack(g);
}

function new_menu()
{
	var id = 'menu' + newId();
	var $menu = $ ("<div id='" + id +"' class='kgm_base kgm_menu'>menu</div>");
	$("#palette").append($menu);
    $menu.menu()
	$menu.draggable();
    $menu.onclick(function(){
        selected = $menu;
        gui_options($menu);
    });

    var g = { target: $menu,
            };

    guis.pushBack(g);
}

function new_list()
{
    var id = "list" + newId();
	var $list = $ ("<select id='" + id + "' class='kgm_base kgm_list' size='5'></select>");
	$("#palette").append($list);
    $list.draggable({cancel: false});
    $list.onclick(function(){
        selected = $list;
        gui_options($list);
    });

    var g = { target: $list,
            };

    guis.pushBack(g);
}

function new_text()
{
    var id = "text" + newId();
	var $text = $ ("<input id='" + id + "' class='kgm_base kgm_text' type='text' value='Text'></input>");
	$("#palette").append($text);
    $text.draggable({cancel: false});
    $text.onclick(function(){
    selected = $text;
        gui_options($text);
    });

    var g = { target: $text,
            };

    guis.pushBack(g);
}

function new_check()
{
    var id = "check" + newId();
    var $div = $("<div id='div" + id + "'></div>");
    var $check = $ ("<input id='" + id + "' class='ui-checkboxradio kgm_base kgm_check' type='checkbox' value='Check'>Check</input>");
    $div.append($check);
    $("#palette").append($div);
    $div.draggable();
    $check.onclick(function(){
        selected = $check;
        gui_options($check);
    });

    var g = { target: $check,
            };

    guis.pushBack(g);
}

function new_label()
{
    var id = "label" + newId();
    var $label = $("<div id='" + id + "' class='ui-state-highlight kgm_base kgm_label'>Label</div>");
    $("#palette").append($label);
    $label.draggable();
    $label.onclick(function(){
        selected = $label;
        gui_options($label);
    });

    var g = { target: $label,
            };

    guis.pushBack(g);
}

function new_button()
{
    var id = "button" + newId();
	var $button = $ ("<button id='" + id + "' class='ui-button kgm_base kgm_button'>Button</button>");
	$("#palette").append($button);
    $button.draggable({cancel: false});
    $button.onclick(function(){
        selected = $button;
        gui_options($button);
    });

    var g = { target: $label,
            };

    guis.pushBack(g);
}

function new_progress()
{
    var id = "progress" + newId();
	var $progress = $ ("<div id='" + id + "' class='kgm_base kgm_progress'></div>");
	$("#palette").append($progress);
    $progress.progressbar({value: 45});
    $progress.draggable();
    $progress.onclick(function(){
        selected = $progress;
        gui_options($progress);
    });

    var g = { target: $progress,
            };

    guis.pushBack(g);
}

function on_palette()
{
    selected = null;
}

function kgm_init()
{
  //$("#new_gui").click(new_gui);
  $("#new_menu").click(new_menu);
  $("#new_list").click(new_list);
  $("#new_text").click(new_text);
  $("#new_check").click(new_check);
  $("#new_label").click(new_label);
  $("#new_button").click(new_button);
  $("#new_progress").click(new_progress);

  $("#palette").click(on_palette);
}
