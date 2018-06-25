var id_num = 0;

function newId()
{
	return id_num++;
}

function gui_move(event)
{
    $palette = $("#palette");

    var dx = $("#" + event.target.id).position.left - $palette.position().left;
    var dy = $("#" + event.target.id).position.top - $palette.position().top;
  	console.log("lol " + event.target.id + " " + event.pageX + " " + event.pageY);
}

function new_gui()
{
	var $gui = $ ("<div class='kgm_gui kgm_base'>gui</div>");
	$("#palette").append($gui);
}

function new_menu()
{
	var id = 'menu' + newId();
	var $menu = $ ("<div id='" + id +"' class='kgm_menu kgm_base'>menu</div>");
	$("#palette").append($menu);
	$("#" + id + "").mousemove(function (event) { 
		gui_move(event); 
	});
}

function new_list()
{
	var $list = $ ("<div class='kgm_list kgm_base'>list</div>");
	$("#palette").append($list);
}

function new_text()
{
	var $text = $ ("<div class='kgm_text kgm_base'>text</div>");
	$("#palette").append($text);
}

function new_check()
{
	var $check = $ ("<div class='kgm_list kgm_base'>check</div>");
	$("#palette").append($check);
}

function new_button()
{
	var $button = $ ("<div class='kgm_button kgm_base'>button</div>");
	$("#palette").append($button);
}

function new_progress()
{
	var $progress = $ ("<div class='kgm_progress kgm_base'>progress</div>");
	$("#palette").append($progress);
}

function kgm_init()
{
	//$("#new_gui").click(new_gui);
	$("#new_menu").click(new_menu);
	$("#new_list").click(new_list);
	$("#new_text").click(new_text);
	$("#new_check").click(new_check);
	$("#new_button").click(new_button);
	$("#new_progress").click(new_progress);
}