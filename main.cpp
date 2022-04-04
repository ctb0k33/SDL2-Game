#include"CommonFunction.h"
#include"BaseObject.h"
#include"game_map.h"
#include"MainObject.h";
#include"ImpTimer.h"
#include"ThreatsObject.h"
#include"ExplosionObject.h"
#include"PlayerPower.h"
#include"TextObject.h"
#include"Geometric.h"
#include"BossObject.h"

BaseObject g_background;
TTF_Font* font_time=NULL;

bool InitData() {
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0) {
		return false;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	g_window = SDL_CreateWindow(" SDL2.0 C++ Game", 
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								SCREEN_WIDTH, SCREEN_HEIGHT,
								SDL_WINDOW_SHOWN);
	if (g_window == NULL) {
		success= false;
	}
	else {
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL) {
			success = false;
		}
		else {
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags)) {
				success = false;
			}
		}
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
			return false;
		}
		//Read audio 
		g_game_sound[0] = Mix_LoadWAV("Debug//sound//gamesound.wav");
		g_sound_bullet[0] = Mix_LoadWAV("Debug//sound//Gun+Silencer.wav");
		g_sound_bullet[1] = Mix_LoadWAV("Debug//sound//Explosion+1.wav");
		g_sound_bullet[2] = Mix_LoadWAV("Debug//sound//Explosion+3.wav");
		g_sound_exp[0] = Mix_LoadWAV("Debug//sound//Recording (6).wav");
		g_coin[0] = Mix_LoadWAV("Debug//sound//coin.wav");
		if (g_sound_bullet[0] == NULL || g_sound_bullet[1] == NULL||g_sound_bullet[2]==NULL||g_game_sound[0]==NULL) {
			return false;
		}
		if (TTF_Init() == -1) {
			success = false;
		}
		font_time = TTF_OpenFont("Debug//font//dlxfont_.ttf.ttf", 15);
		if (font_time == NULL) {
			success = false;
		}



	}
	return success;
}
bool LoadBackground() {
	bool ret = g_background.LoadImg("Debug//image//background4.png", g_screen);
	if (ret == false) {
		return false;
	}
	return true;
}
void close() {
	g_background.Free();

	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;
	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();
}

std::vector<ThreatsObject*> MakeThreadList() {
	std::vector<ThreatsObject*> list_threats;

	ThreatsObject* dynamic_threats = new ThreatsObject[40];
	for (int i = 1; i < 40; i++) {
		ThreatsObject* p_threat = (dynamic_threats + i);
		if (p_threat != NULL) {
			p_threat->LoadImg("Debug//image//threat_left.png", g_screen);
			p_threat->Set_clips();
			p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
			p_threat->set_x_pos(500 + i * 500);
			p_threat->set_y_pos(200);

			int pos1 = p_threat->get_x_pos() - 60;
			int pos2 = p_threat->get_x_pos() + 60;
			p_threat->SetAnimationPos(pos1, pos2);
			p_threat->set_input_left(1);
			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen);
			list_threats.push_back(p_threat);
		}
	}

	ThreatsObject* threats_objs = new ThreatsObject[20];

	for (int i = 0; i < 20; i++) {
		ThreatsObject* p_threat = (threats_objs + i);
		if (p_threat != NULL) {
			p_threat->LoadImg("Debug//image//threat_level.png", g_screen);
			p_threat->Set_clips();
			p_threat->set_x_pos(700+i*1200);
			p_threat->set_y_pos(250);
			p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p_threat->set_input_left(0);
			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen);
			list_threats.push_back(p_threat);
		}
	}

	return list_threats;
}
int main(int argc, char* agrv[]) {
	bool game_win = false;
	ImpTimer fps_timer;

	if (InitData() == false)
		return -1;
	if (LoadBackground() == false)
		return -1;


	GameMap game_map;
	game_map.LoadMap("map_type4//map.dat");
	game_map.LoadTiles(g_screen);


	MainObject p_player;
	p_player.LoadImg("Debug//image//player_right.png", g_screen);
	p_player.set_clips();

	PlayerPower player_power;
	player_power.Init(g_screen);

	PlayerMoney player_money;
	player_money.Init(g_screen);
	player_money.SetPos(SCREEN_WIDTH * 0.5-285, 7);

	std::vector<ThreatsObject*> threats_list = MakeThreadList();

	// Boss Threart
	BossObject bossObject;
	bossObject.LoadImg("Debug//image//boss_object.png", g_screen);
	bossObject.set_clips();
	int xPosBoss = MAX_MAP_X * TILE_SIZE -SCREEN_WIDTH*0.6;
	bossObject.set_xpos(xPosBoss);
	bossObject.set_ypos(10);

	BossHp boss_hp;

	//explosion
	ExplosionObject exp_threat,exp_main;
	bool tRet = exp_threat.LoadImg("Debug//image//exp1.png", g_screen);
	if (tRet == false) {
		return -1;
	}
	bool mRet = exp_main.LoadImg("Debug//image//exp2.png", g_screen);
	if (mRet == false) {
		return -1;
	}
	exp_main.set_clip();
	exp_threat.set_clip();
	
	int life=player_power.GetNum();

	//Time text
	TextObject time_game;
	time_game.SetColor(TextObject::WHILE_TEXT);

	TextObject mark_game;
	mark_game.SetColor(TextObject::WHILE_TEXT);
	UINT mark_value = 0;


	TextObject money_game;
	money_game.SetColor(TextObject::WHILE_TEXT);

	TextObject boss_name;
	boss_name.SetColor(TextObject::BLACK_TEXT);
	int bulletcheck = 0; // check for boss
	int hit_boss = 0; // boss life
	int condition_appear = 0; // boss appear

	bool is_quit = false;
	Mix_PlayChannel(-1, g_game_sound[0], 0);
	while (!is_quit)
	{
		if (game_win == true) {
			if (MessageBox(NULL, L"YOU WIN", L"Info", MB_OK | MB_ICONSTOP) == IDOK) {
				is_quit = true;
				break;
			}
		}
		fps_timer.start();
		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT) {
				is_quit = true;
			}
			p_player.HandleInputAction(g_event, g_screen,g_sound_bullet,g_sound_exp);
		}
		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);

		g_background.Render(g_screen, NULL);


		Map map_data = game_map.getMap();

		p_player.HandleBullet(g_screen); // +x_val for bullet
		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
		p_player.DoPlayer(map_data, g_coin);
		p_player.Show(g_screen);
		game_map.SetMap(map_data);
		game_map.DrawMap(g_screen);

		//Draw Geometric
		GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40);
		ColorData color_data(36, 36, 36);
		Geometric::RenderRecttangle(rectangle_size, color_data, g_screen);

		GeometricFormat outLineSize(1, 1, SCREEN_WIDTH - 1, 38);
		ColorData  color_data2(255, 255, 255);

		Geometric::RenderOutline(outLineSize, color_data2, g_screen);

		player_power.Show(g_screen);
		player_money.Show(g_screen);

		// Check falling into cliff
		if (p_player.GetRectFrame().y > SCREEN_HEIGHT - 10) {
			life--;
			if (life >= 0) {
				p_player.SetRect(0, 0);
				p_player.set_comeback_time(60);
				player_power.Decrease();
				player_power.Render(g_screen);
			}
			else {
				if (MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK) {
					close();
					SDL_Quit();
					return 0;
				}
			}
		}
		// CheckCollision
		for (int i = 0; i < threats_list.size(); i++) {
			ThreatsObject* p_threat = threats_list.at(i);
			if (p_threat != NULL) {
				p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
				p_threat->ImpMoveType(g_screen);
				p_threat->DoPlayer(map_data);
				if (p_threat->get_direct() == 0) {
					p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				}
				/*else*/ if (p_threat->get_direct() == 1) {
					p_threat->MakeBullet2(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				}
				p_threat->Show(g_screen);
				SDL_Rect rect_player = p_player.GetRectFrame();
				bool bCol1 = false;
				std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
				for (int jj = 0; jj < tBullet_list.size(); ++jj) {
					BulletObject* pt_bullet = tBullet_list.at(jj);
					if (pt_bullet) {
						bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
						if (bCol1) {
							exp_main.start_explosion(p_player.GetRect(), g_screen, exp_main);
							SDL_RenderPresent(g_screen);
							/*p_threat->RemoveBulelt(jj);*/
							Mix_PlayChannel(-1, g_sound_bullet[2], 0);
							break;
						}
					}
				}
				SDL_Rect rect_threat = p_threat->GetRectFrame();
				bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
				if (bCol1 || bCol2) {
					exp_main.start_explosion(p_player.GetRect(), g_screen, exp_main);
					SDL_RenderPresent(g_screen);
					/*	exp_main*/
					life--;
					if (life >= 0) {
						Mix_PlayChannel(-1, g_sound_bullet[2], 0);
						p_player.SetRect(0, 0);
						p_player.set_comeback_time(60);
						SDL_Delay(1000);
						player_power.Decrease();
						player_power.Render(g_screen);
						continue;
					}
					else {
						if (MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK) {
							Mix_PlayChannel(-1, g_sound_bullet[2], 0);
							p_threat->Free();
							close();
							SDL_Quit();
							return 0;
						}
					}
				}
			}
		}

		std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
		for (int r = 0; r < bullet_arr.size(); r++) {
			BulletObject* p_bullet = bullet_arr.at(r);
			if (p_bullet != NULL) {
				for (int t = 0; t < threats_list.size(); t++) {
					ThreatsObject* obj_threat = threats_list.at(t);
					if (obj_threat != NULL) {
						SDL_Rect tRect;
						tRect.x = obj_threat->GetRect().x;
						tRect.y = obj_threat->GetRect().y;
						tRect.w = obj_threat->get_width_frame_();
						tRect.h = obj_threat->get_height_frame_();

						SDL_Rect bRect = p_bullet->GetRect();

						bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);
						if (bCol) {
							/*	explosion*/
							Mix_PlayChannel(-1, g_sound_bullet[1], 0);
							mark_value++;
							exp_threat.start_explosion(bRect, g_screen, exp_threat);
							p_player.RemoveBulelt(r);
							obj_threat->Free();
							threats_list.erase(threats_list.begin() + t);
						}
					}
				}
			}
		}

		//show game time , show text

		std::string str_time = "Time: ";
		Uint32 time_val_ = SDL_GetTicks() / 1000;
		Uint32 val_time = 300 - time_val_;
		if (val_time <= 0) {
			if (MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK) {
				is_quit = true;
				break;
			}
		}
		else {
			std::string str_val = std::to_string(val_time);
			str_time += str_val;

			time_game.SetText(str_time);
			time_game.LoadFromRenderText(font_time, g_screen);
			time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
		}

		std::string val_str_mark = std::to_string(mark_value);
		std::string strMark("Mark: ");
		strMark += val_str_mark;

		mark_game.SetText(strMark);
		mark_game.LoadFromRenderText(font_time, g_screen);
		mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 50, 15);

		int money_count = p_player.GetMoneyCount();
		std::string money_str = std::to_string(money_count);

		if (money_count >= 10) {
			life++;
			player_power.InitCrease();
			player_power.Show(g_screen);
			p_player.DecreaseMoney();
		}
		money_game.SetText(money_str);
		money_game.LoadFromRenderText(font_time, g_screen);
		money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 250, 15);
		/* show boss */
		std::string dragon_name = "Stormterror Dvalin";
		boss_name.SetText(dragon_name);
		int val = MAX_MAP_X * TILE_SIZE - (map_data.start_x_ + p_player.GetRect().x);
		if (val <= SCREEN_WIDTH) {
			condition_appear = 1;
		}
		if (condition_appear==1) {
		/*	map_data.start_x_ = bossObject.get_x_pos() - SCREEN_WIDTH;
			if (p_player.get_x_pos_() < map_data.start_x_) {
				p_player.set_x_val_();
			}*/
			boss_name.LoadFromRenderText(font_time, g_screen);
			boss_name.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 100, 42);
			boss_hp.Init(g_screen);
			boss_hp.SetRect(SCREEN_WIDTH/2-400, 60);
			boss_hp.Show(g_screen);
			bossObject.SetMapXY(map_data.start_x_, map_data.start_y_);
			bossObject.DoPlayer(map_data);
			if (bulletcheck == 0) {
				bossObject.MakeBulelt(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
			}
			bossObject.Show(g_screen);
			/*	CheckCollision player and boos*/
			for (int i = 0; i < bullet_arr.size(); i++) {
				BulletObject* p_bullet = bullet_arr.at(i);
				bool bcol3 = SDLCommonFunc::CheckCollision(bossObject.GetRect(), p_bullet->GetRect());
				if (bcol3 == true) {
					boss_hp.Decrease();
					Mix_PlayChannel(-1, g_sound_bullet[1], 0);
					exp_threat.start_explosion(p_bullet->GetRect(), g_screen, exp_threat);
					hit_boss++;
					p_player.RemoveBulelt(i);
					if (hit_boss == 5) {
						exp_threat.start_explosion(p_bullet->GetRect(), g_screen, exp_threat);
						bossObject.RemoveBullet();
						bossObject.Free();
						boss_hp.Init(g_screen);
						boss_hp.Show(g_screen);
						bulletcheck = 1;
						game_win = true;
						SDL_Delay(1000);
						
					}
				}
			}
			bool bcol4 = SDLCommonFunc::CheckCollision(p_player.GetRectFrame(), bossObject.GetRect());
			for (int i = 0; i < bossObject.get_bullet_list().size(); i++) {
				BulletObject* b_bullet = bossObject.get_bullet_list().at(i);
				bool bcol5 = SDLCommonFunc::CheckCollision(p_player.GetRectFrame(), b_bullet->GetRect());
				if (bcol5 || bcol4) {
					Mix_PlayChannel(-1, g_sound_bullet[2], 0);
					exp_main.start_explosion(p_player.GetRect(), g_screen, exp_main);
					SDL_RenderPresent(g_screen);
					/*	exp_main*/
					life--;
					if (life >= 0) {
						p_player.SetRect(0, 0);
						p_player.set_comeback_time(60);
						SDL_Delay(1000);
						player_power.Decrease();
						player_power.Render(g_screen);
						continue;
					}
					else {
						if (MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK) {
							close();
							SDL_Quit();
							return 0;
						}
					}
				}
			}
		}
			SDL_RenderPresent(g_screen);

			int real_imp_time = fps_timer.get_ticks();// lay thoi gian thuc te
			int time_one_frame = 1000 / FRAME_PER_SECOND; //ms
			if (real_imp_time < time_one_frame) {
				int delay_time = time_one_frame - real_imp_time;
				if (delay_time >= 0)
					SDL_Delay(delay_time);// tao do tre
			}
		}
		for (int i = 0; i < threats_list.size(); i++) {
			ThreatsObject* p_threat = threats_list.at(i);
			if (p_threat) {
				p_threat->Free();
				p_threat = NULL;
			}
		}
	threats_list.clear();

	close();
	return 0;
}










