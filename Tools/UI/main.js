var id_num = 0;

var gui_selected = null;

var guis = []

function newId()
{
	return id_num++;
}

function gui_sizer_show()
{
    $('#gui_size').show();
}

function gui_sizer_hide()
{
    $('#gui_size').hide();
}

function gui_sizer_move(x, y)
{
    $('#gui_size').css('left', x);
    $('#gui_size').css('top', y);
}

function gui_sizer_drag(e)
{
    if(!gui_selected)
        return;

    var w = e.pageX - gui_selected.position().left - $('#palette').position().left;
    var h = e.pageY - gui_selected.position().top - $('#palette').position().top;

    gui_selected.css('width', w);
    gui_selected.css('height', h);
}

function gui_get_by_target(g)
{
    var i;

    for(i = 0; i < guis.length; i++)
    {
        var c = guis[i];

        if (c.target == g)
            return c;
    }

    return null;
}

function gui_options(gui)
{
    if (gui == null)
    {
      $("#options").empty();

      return;
    } 
    
    gui_selected = gui;

    var gui_data = gui_get_by_target(gui);

    $("#options").empty();

    $table = $("<table id='tboptions'></table>");
    $("#options").append($table);

    $table.append("<tr><td>x</td><td><input size='15' id='gui_x' type='text' value='" + gui.position().left + "'/></td></tr>");
    $table.append("<tr><td>y</td><td><input size='15' id='gui_y' type='text' value='" + gui.position().top + "'/></td></tr>");
    $table.append("<tr><td>w</td><td><input size='15' id='gui_w' type='text' value='" + gui.width() + "'/></td></tr>");
    $table.append("<tr><td>h</td><td><input size='15' id='gui_h' type='text' value='" + gui.height() + "'/></td></tr>");
    $table.append("<tr><td>handler</td><td><input size='15' id='gui_handler' type='text' value='" +
                  gui_data.handler + "' readonly/></td></tr>");

    if(gui_data.type != 'list' && gui_data.type != 'menu' && gui_data.type != 'progress' && gui_data.type != 'text')
        $table.append("<tr><td>text</td><td><input size='15' id='gui_text' type='text' value='" +
                      gui_data.text + "'/></td></tr>");

    $("#options").append("<button id='cmd_gui_delete'>delete</button>");
    $("#cmd_gui_delete").click(function(){
        var c = gui_get_by_target(gui_selected);
        var ci = 0;

        while(guis[ci] != c)
            ci++;
        guis.splice(ci, 1);
        gui_selected.remove();
        gui_selected = null;
        $("#options").empty();
    });

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
    $('#gui_text').on('input', function(){
        var c = gui_get_by_target(gui_selected);
        c.text = $('#gui_text').val();

        if (c.type == 'text')
            c.target.val(c.text);
        else if (c.type == 'check')
            c.target[0].childNodes[1].data = c.text;
        else if (c.type == 'label')
            c.target.text(c.text);
        else if (c.type == 'button')
            c.target.text(c.text);
    });
    $('#gui_handler').on('input', function(){
        var c = gui_get_by_target(gui_selected);
        c.handler = $('#gui_handler').val();
    });

    var x = $('#palette').position().left + gui.position().left + gui.width();
    var y = $('#palette').position().top + gui.position().top  + gui.height();

    gui_sizer_move(x, y);
    gui_sizer_show();
}

function gui_list_menu_add()
{
    $("#gui_list_menu").hide();
    $("#gui_list_menu_add_dialog").dialog('open');
}

function gui_list_menu_remove()
{
    $("#gui_list_menu").hide();

    gui_selected.each(function(i, el){
        for (var j = el.options.length; j > 0; j--)
        {
            var o = this.options[j - 1];

            if (o.selected)
            {
                $(o).remove();
            }
        }
    });
}

function gui_list_menu_dragging()
{
    $("#gui_list_menu").hide();

    var c = gui_get_by_target(gui_selected);

    if (c.type != 'list')
        return;

    if (c.dragging == undefined)
    {
        c.dragging = true;
    }
    else
    {
        c.dragging = !c.dragging;
    }

    if (c.dragging)
    {
        c.target.draggable({cancel: false});
        c.target.draggable('enable');
        $("#gui_list_menu_dragging").text('undrag');
    }
    else
    {
        c.target.draggable();
        c.target.draggable('disable');
        $("#gui_list_menu_dragging").text('drag');
    }
}

function gui_list_add_text()
{
    gui_selected.append("<option>" + $("#gui_list_text_for_add").val() + "</option>");
    $("#gui_list_menu_add_dialog").dialog('close');
}

function gui_list_remove_text()
{
}

function gui_list_context(e)
{
    if (gui_selected == null)
        return;

    $("#gui_list_menu").show();
    $("#gui_list_menu").css('left', e.clientX);
    $("#gui_list_menu").css('top', e.clientY);

}

function gui_tabbar_menu_add()
{
    $("#gui_tabbar_menu").hide();
    $("#gui_tabbar_menu_add_dialog").dialog('open');
}

function gui_tabbar_menu_remove()
{
    $("#gui_tabbar_menu").hide();

    gui_selected.each(function(i, el){
        for (var j = el.options.length; j > 0; j--)
        {
            var o = this.options[j - 1];

            if (o.selected)
            {
                $(o).remove();
            }
        }
    });
}

function gui_tabbar_add_tab()
{
  var menu = gui_selected.find('tab_menu');
  
  menu = gui_selected.children('tab_menu').first();
	
  if (menu != null)
  {
    $('#tab_menu').append("<li>" + $("#gui_tabbar_text_for_add").val() + "</li>");
    //gui_selected.tabs();
  }

  $("#gui_tabbar_menu_add_dialog").dialog('close');
}

function gui_tabbar_context(e)
{
    if (gui_selected == null)
        return;

    $("#gui_tabbar_menu").show();
    $("#gui_tabbar_menu").css('left', e.clientX);
    $("#gui_tabbar_menu").css('top', e.clientY);

}

function gui_menu_context(e)
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
        evt.stopPropagation();
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
    $menu.click(function(evt){
        gui_options($menu);
        evt.stopPropagation();
    });

    $menu.contextmenu(gui_menu_context);

    var g = { target: $menu,
              id: id,
              type: 'menu',
              handler: 'onGuiMenuHandler'
            };

    guis.push(g);
}

function new_list()
{
    var id = "list" + newId();
    var $list = $ ("<select id='" + id + "' class='kgm_base kgm_list'size='5'></select>");
    $("#palette").append($list);
    $list.draggable(); //{cancel: false}
    $list.click(function(evt){
      gui_options($list);
      evt.stopPropagation();
    });
    $list.contextmenu(gui_list_context);

    var g = { target: $list,
              id: id,
              type: 'list',
              handler: 'onGuiListHandler'
            };

    guis.push(g);
}

function new_text()
{
    var id = "text" + newId();
    var $text = $ ("<input id='" + id + "' class='kgm_base kgm_text' type='text' value='Text'></input>");
    $("#palette").append($text);
    $text.draggable({cancel: false});
    $text.click(function(evt){
      selected = $text;
      gui_options($text);
      evt.stopPropagation();
    });

    var g = { target: $text,
              id: id,
              type: 'text',
              handler: 'onGuiTextHandler',
              text: $text.val()
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
    $div.click(function(evt){
        gui_options($div);
        evt.stopPropagation();
    });

    var g = { target: $div,
              id: id,
              type: 'check',
              handler: 'onGuiCheckHandler',
              text: $div.text()
            };

    guis.push(g);
}

function new_label()
{
    var id = "label" + newId();
    var $label = $("<div id='" + id + "' class='ui-state-highlight kgm_base kgm_label'>Label</div>");
    $("#palette").append($label);
    $label.draggable();
    $label.click(function(evt){
        gui_options($label);
        evt.stopPropagation();
    });

    var g = { target: $label,
              id: id,
              type: 'label',
              handler: 'onGuiLabelHandler',
              text: $label.text()
            };

    guis.push(g);
}

function new_button()
{
    var id = "button" + newId();
    var $button = $ ("<button id='" + id + "' class='ui-button kgm_base kgm_button'>Button</button>");
    $("#palette").append($button);
    $button.draggable({cancel: false});
    $button.click(function(evt){
        gui_options($button);
        evt.stopPropagation();
    });

    var g = { target: $button,
              id: id,
              type: 'button',
              handler: 'onGuiButtonHandler'
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
    $progress.click(function(evt){
        gui_options($progress);
        evt.stopPropagation();
    });

    var g = { target: $progress,
              id: id,
              type: 'progress',
              handler: 'onGuiProgressHandler'
            };

    guis.push(g);
}

function new_tabbar()
{
    var id = "tabbar" + newId();
	var $tabbar = $ ("<div id='" + id + "' class='kgm_base kgm_tabbar'><ul id='tab_menu'></ul></div>");
    $("#palette").append($tabbar);
    $tabbar.tabs();
    $tabbar.draggable();
    $tabbar.click(function(evt){
        gui_options($tabbar);
        evt.stopPropagation();
    });
    
    $tabbar.contextmenu(gui_tabbar_context);

    var g = { target: $tabbar,
              id: id,
              type: 'tabbar',
              handler: 'onGuiTabbarHandler'
            };

    guis.push(g);
}

function on_palette()
{
    selected = null;

    gui_sizer_hide();
    
    var gui = $("palette");

    $("#gui_list_menu").hide();
    
    $("#options").empty();

    $table = $("<table id='tboptions'></table>");
    $("#options").append($table);

    $table.append("<tr><td>x</td><td><input readonly size='15' id='gui_x' type='text' value='" + this.offsetLeft + "'/></td></tr>");
    $table.append("<tr><td>y</td><td><input readonly size='15' id='gui_y' type='text' value='" + this.offsetTop + "'/></td></tr>");
    $table.append("<tr><td>w</td><td><input readonly size='15' id='gui_w' type='text' value='" + this.clientWidth + "'/></td></tr>");
    $table.append("<tr><td>h</td><td><input readonly size='15' id='gui_h' type='text' value='" + this.clientHeight + "'/></td></tr>");

}

function build_gui_xml()
{
    var text = "<?xml version=1.0?>\n";

    text += "<kgmGui x='0' y='0' w='" + $("#palette").width() + "' h='" + $("#palette").height() + "'>\n";

    for (var i = 0; i < guis.length; i++)
    {
        var g = guis[i];

        var gtype = "kgmGui";

        var x = g.target.position().left;
        var y = g.target.position().top;
        var w = g.target.width();
        var h = g.target.height();

        var gtext = null;
        var gitms = null;
        var ghand = g.handler;

        var gclose = true;

        if (g.type == 'list')
        {
            gtype = 'kgmGuiList';
            gitms = new Array();
            gclose = false;
        }
        else if (g.type == 'text')
        {
            gtype = 'kgmGuiText';
            gtext = g.target.val();
        }
        else if (g.type == 'check')
        {
            gtype = 'kgmGuiCheck';
        }
        else if (g.type == 'label')
        {
            gtype = 'kgmGuiLabel';
            gtext = g.target.text();
        }
        else if (g.type == 'button')
        {
            gtype = 'kgmGuiButton';
            gtext = g.target.text();
        }
        else if (g.type == 'progress')
        {
            gtype = 'kgmGuiProgress';
        }

        text += " <" + gtype + " id='" + g.id + "' x='" + x + "' y='" + y + "' w='" + w + "' h='" + h + "'";
        text += " handler='" + ghand + "'";

        if (gtext != null)
            text += " text='" + gtext + "'";

        if (gclose)
        {
            text += " />\n";
        }
        else
        {
            text += " >\n";

            if (g.type == 'list')
            {
                var el = g.target[0];

                for (var j = 0; j < el.options.length; j++)
                {
                    var o = el.options[j];

                    text += "  <ListItem text='" + $(o).text() + "' />\n";
                }

            }

            text += " </" + gtype + ">\n";
        }
    }

    text += "</kgmGui>\n";

    return text;
}

function cmd_save(name)
{
    var textToWrite = build_gui_xml();
    var textFileAsBlob = new Blob([textToWrite], {type:'text/plain'});
    var fileNameToSaveAs = name; //document.getElementById("inputFileNameToSaveAs").value;
    var downloadLink = document.createElement("a");
    downloadLink.download = fileNameToSaveAs;
    downloadLink.innerHTML = "Download File";

    if (window.webkitURL != null)
    {
        downloadLink.href = window.webkitURL.createObjectURL(textFileAsBlob);
    }
    else
    {
        downloadLink.href = window.URL.createObjectURL(textFileAsBlob);
        downloadLink.onclick = destroyClickedElement;
        downloadLink.style.display = "none";
        document.body.appendChild(downloadLink);
    }

    downloadLink.click();

    $('#cmd_save_dialog').dialog('destroy').remove();
}

function cmd_save_dialog()
{
    var div = $("<div id='cmd_save_dialog'><input type='text' id='cmd_save_dialog_text' value='gui_layout.ui'/><button id='cmd_save_dialog_save' value='save'>save</button></div>");
    $(document.body).append(div);
    $('#cmd_save_dialog_save').click(function(){
        cmd_save($('#cmd_save_dialog_text').val());
    });
    $('#cmd_save_dialog').dialog({
        title: 'Save',
        close: function(event, ui)
        {
            $(this).dialog('destroy').remove();
        }
    });
    $('#cmd_save_dialog').dialog('open');
}

function kgm_init()
{
    //$("#new_gui").click(new_gui);
    //$("#new_menu").click(new_menu);
    $("#new_list").click(new_list);
    $("#new_text").click(new_text);
    $("#new_check").click(new_check);
    $("#new_label").click(new_label);
    $("#new_button").click(new_button);
    $("#new_progress").click(new_progress);
    $("#new_tabbar").click(new_tabbar);

    $("#palette").click(on_palette);

    $("#gui_size").draggable({
        drag: gui_sizer_drag
    });
    gui_sizer_hide();

    $("#gui_list_menu").menu();
    $("#gui_list_menu_add").click(gui_list_menu_add);
    $("#gui_list_menu_remove").click(gui_list_menu_remove);
    $("#gui_list_menu_dragging").click(gui_list_menu_dragging);
    $("#gui_list_menu_add_dialog").dialog();
    $("#gui_list_menu_add_dialog").dialog('close');
    $("#gui_list_add_text").click(gui_list_add_text);

    $("#gui_tabbar_menu").menu();
    $("#gui_tabbar_menu_add").click(gui_tabbar_menu_add);
    $("#gui_tabbar_menu_remove").click(gui_tabbar_menu_remove);
    $("#gui_tabbar_menu_add_dialog").dialog();
    $("#gui_tabbar_menu_add_dialog").dialog('close');
    $("#gui_tabbar_add_tab").click(gui_tabbar_add_tab);

    $("#cmd_save").click(cmd_save_dialog);
}
