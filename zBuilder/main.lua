--g=_gui(0, 0, 0, 300, 300)

GAME_STATE_NONE = 0
GAME_STATE_RUN = 1
GAME_STATE_PAUSE = 2
GAME_STATE_LOADING = 3
GAME_STATE_UNLOADING = 4

gbtn_none = 0
gbtn_up = 1
gbtn_down = 2
gbtn_left = 3
gbtn_right = 4
gbtn_exit = 5
gbtn_pause = 6
gbtn_start = 7
gbtn_a = 8
gbtn_b = 9
gbtn_c = 10
gbtn_d = 11
gbtn_e = 12
gbtn_f = 13
gbtn_g = 14
gbtn_h = 15
gbtn_i = 16
gbtn_j = 17
gbtn_k = 18
gbtn_l = 19
gbtn_m = 20
gbtn_n = 21
gbtn_o = 22
gbtn_p = 23
gbtn_q = 24
gbtn_r = 25
gbtn_s = 26
gbtn_t = 27
gbtn_u = 28
gbtn_v = 29
gbtn_w = 30
gbtn_x = 31
gbtn_y = 32
gbtn_z = 33

 
game_state = GAME_STATE_NONE

mainGui = Gui.new(nil, 0, 0, 300, 300)
gameGui = Gui.new(mainGui, 0, 50, 300, 280)
setsGui = Gui.new(mainGui, 0, 50, 300, 280)

gameBtn = GuiButton.new(mainGui, 0, 0, 99, 20, 'Game')
setsBtn = GuiButton.new(mainGui, 100, 0, 99, 20, 'Settings')
quitBtn = GuiButton.new(mainGui, 200, 0, 99, 20, 'Quit')

gameGui:show()
setsGui:hide()

glst = GuiList.new(gameGui, 10, 40, 200, 300)
glst:add('hello0')
glst:add('hello1')
glst:add('hello2')
glst:add('hello3')
glst:add('level01')

maps = {'map001.kgm', 
        'map002.kgm', 
        'map003.kgm',
        'map004.kgm',
        'level01.kgm',
	'map006.kgm', 
	'map007.kgm', 
	'map008.kgm', 
	'map009.kgm'}
--
--
--

_game_add_idle("_game_idle")



function _gui_action(g, i)
	if g == gameBtn.gui then
		print "Game button click..."
                setsGui:hide()
                gameGui:show()
	elseif g == setsBtn.gui then
		print "Settings button click..."
                gameGui:hide()
                setsGui:show()
	elseif g == quitBtn.gui then
		print "Quit button click..."
                _game_command('quit')
	elseif g == glst.gui then
		print "list"
		cmd = 'load ' .. maps[i+1]
		print (cmd)
                game_state = GAME_STATE_LOADING
		res = _game_command(cmd)
		print ('result is: ' .. res)
                if res == 1 then
                    print ('try to hide ')
                    mainGui:hide();
                    game_state = GAME_STATE_RUN
                else
                    game_state = GAME_STATE_NONE
                end
	end
	--print(i)
end

function _game_action(a)
	if a == 'loading' then
	elseif a == 'load' then
	elseif a == 'unload' then
	elseif a == 'pause' then
	end
end

function _game_input(act, arg)	
    if game_state == GAME_STATE_NONE then
        return
    end

    if game_state == GAME_STATE_PAUSE then
        if act == gbtn_exit and arg == 0 then
            game_state = GAME_STATE_RUN
        end
    end

    if game_state == GAME_STATE_RUN then
        if act == gbtn_exit and arg == 0 then
            game_state = GAME_STATE_UNLOADING
            res = _game_command('unload')
            game_state = GAME_STATE_NONE
            mainGui:show()
        end

        if act == gbtn_up and arg == 1 then
           _game_camera_move(0.1)
        end

        if act == gbtn_down and arg == 1 then
           _game_camera_move(-0.1)
        end

        if act == gbtn_left and arg == 1 then
           _game_camera_rotate(0.03, 0.0, 0.0)
        end

        if act == gbtn_right and arg == 1 then
            _game_camera_rotate(-0.03, 0.0, 0.0)
        end

        if act == gbtn_x and arg == 1 then
            _game_camera_rotate(0.0, -0.01, 0.0)
            print('x')
        end

        if act == gbtn_z and arg == 1 then
            _game_camera_rotate(0.0, 0.01, 0.0)
            print('z')
        end
    end

    --a = '_game_input' .. act .. ' ' .. arg;
    --print(a)
end

function _game_idle()
  --a = '_game_idle';
  --print(a)
end
