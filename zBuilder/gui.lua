Gui = {}
Gui.__index = Gui


function Gui.new(par, x, y, w, h)
	local _lgui = {}
	setmetatable(_lgui, Gui)
	
	if par == nil then
		_lgui.gui = _gui(0, x, y, w, h)
	else
		_lgui.gui = _gui(par.gui, x, y, w, h)	
	end

	print('Gui')
	print('generate gui')
	return _lgui
end

function Gui:show()
	_gui_show(self.gui, 1)
end

function Gui:hide()
        print ('hiding gui');
	_gui_show(self.gui, 0)
end


GuiButton = {}
GuiButton.__index = GuiButton

function GuiButton.new(par, x, y, w, h, txt)
	local _lgui = {}
	setmetatable(_lgui, GuiButton)
	
	if par ~= nil then
		_lgui.gui = _gui_button(par.gui, x, y, w, h, txt)
	end
	print('generate guibutton')
	return _lgui
end


GuiList = {}
GuiList.__index = GuiList

function GuiList.new(par, x, y, w, h)
	local _lgui = {}
	setmetatable(_lgui, GuiList)
	
	if par ~= nil then
		_lgui.gui = _gui_list(par.gui, x, y, w, h)
	end
	print('generate guilist')
	return _lgui
end

function GuiList:add(txt)
	_gui_list_add_item(self.gui, txt)
	return self
end

function GuiList:remove(item)
	_gui_list_rem_item(self.gui, item)
	return self
end
