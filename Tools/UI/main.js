var id_num = 0;

var gui_selected = null;

var guis = []

function newId()
{
	return id_num++;
}

function gui_options(gui)
{
    if (gui == null || gui_selected == gui)
        return;

    gui_selected = gui;

    $("#options").empty();

    $table = $("<table id='tboptions'></table>");
    $("#options").append($table);

    $table.append("<tr><td>x</td><td><input id='gui_x' type='text' value='" + gui.position().left + "'/></td></tr>");
    $table.append("<tr><td>y</td><td><input id='gui_y' type='text' value='" + gui.position().top + "'/></td></tr>");
    $table.append("<tr><td>w</td><td><input id='gui_w' type='text' value='" + gui.width() + "'/></td></tr>");
    $table.append("<tr><td>h</td><td><input id='gui_h' type='text' value='" + gui.height() + "'/></td></tr>");

    $('#gui_x').on('input', function(){
        gui_selected.offset().left = parseInt($('#gui_x').val());
    });
    $('#gui_y').on('input', function(){
        gui_selected.offset().top = parseInt($('#gui_y').val());
    });
    $('#gui_w').on('input', function(){
        gui_selected.width(parseInt($('#gui_w').val()));
    });
    $('#gui_h').on('input', function(){
        gui_selected.height(parseInt($('#gui_h').val()));
    });
}

function gui_list_context()
{
    alert('Implementing');
}

function gui_menu_context()
{
    alert('Implementing');
}

function new_gui()
{
    var id = "gui" + newId();
	var $gui = $ ("<div class='kgm_base kgm_gui'>gui</div>");
	$("#palette").append($gui);
    $gui.draggable();
    $gui.click(function(){
        gui_options($gui);
    });

    var g = { target: $gui,
              id: id,
              type: 'gui'
            };

    guis.push(g);
}

function new_menu()
{
	var id = 'menu' + newId();
	var $menu = $ ("<div id='" + id +"' class='kgm_base kgm_menu'>menu</div>");
	$("#palette").append($menu);
    $menu.menu()
	$menu.draggable();
    $menu.click(function(){
        gui_options($menu);
    });
    $menu.contextmenu(gui_menu_context);

    var g = { target: $menu,
              id: id,
              type: 'menu'
            };

    guis.push(g);
}

function new_list()
{
    var id = "list" + newId();
	var $list = $ ("<select id='" + id + "' class='kgm_base kgm_list' size='5'></select>");
	$("#palette").append($list);
    $list.draggable({cancel: false});
    $list.click(function(){
        gui_options($list);
    });
    $menu.contextmenu(gui_list_context);

    var g = { target: $list,
              id: id,
              type: 'list'
            };

    guis.push(g);
}

function new_text()
{
    var id = "text" + newId();
	var $text = $ ("<input id='" + id + "' class='kgm_base kgm_text' type='text' value='Text'></input>");
	$("#palette").append($text);
    $text.draggable({cancel: false});
    $text.click(function(){
    selected = $text;
        gui_options($text);
    });

    var g = { target: $text,
              id: id,
              type: 'text'
            };

    guis.push(g);
}

function new_check()
{
    var id = "check" + newId();
    var $div = $("<div id='div" + id + "'  class='ui-checkboxradio kgm_base kgm_check'></div>");
    $div.append($ ("<input class='' type='checkbox'>Check</input>"));
    $("#palette").append($div);
    $div.draggable({cancel: false});
    $div.click(function(){
        gui_options($div);
    });

    var g = { target: $div,
              id: id,
              type: 'check'
            };

    guis.push(g);
}

function new_label()
{
    var id = "label" + newId();
    var $label = $("<div id='" + id + "' class='ui-state-highlight kgm_base kgm_label'>Label</div>");
    $("#palette").append($label);
    $label.draggable();
    $label.click(function(){
        gui_options($label);
    });

    var g = { target: $label,
              id: id,
              type: 'label'
            };

    guis.push(g);
}

function new_button()
{
    var id = "button" + newId();
	var $button = $ ("<button id='" + id + "' class='ui-button kgm_base kgm_button'>Button</button>");
	$("#palette").append($button);
    $button.draggable({cancel: false});
    $button.click(function(){
        gui_options($button);
    });

    var g = { target: $label,
              id: id,
              type: 'button'
            };

    guis.push(g);
}

function new_progress()
{
    var id = "progress" + newId();
	var $progress = $ ("<div id='" + id + "' class='kgm_base kgm_progress'></div>");
	$("#palette").append($progress);
    $progress.progressbar({value: 45});
    $progress.draggable();
    $progress.click(function(){
        gui_options($progress);
    });

    var g = { target: $progress,
              id: id
            };

    guis.push(g);
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
