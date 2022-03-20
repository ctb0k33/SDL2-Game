#include"PlayerPower.h"

PlayerPower::PlayerPower() {
	number_ = 0;
}

PlayerPower::~PlayerPower() {

}
void PlayerPower::AddPos(const int& xp) {
	pos_list_.push_back(xp);
}

void PlayerPower::Init(SDL_Renderer* screen) {
	LoadImg("Debug//image//player_pw.png", screen);
	number_ = 3;
	if (pos_list_.size() > 0) {
		pos_list_.clear();
	}
	AddPos(20);
	AddPos(60);
	AddPos(100);
}

void PlayerPower::Decrease() {
	number_--;
	pos_list_.pop_back();
}

void PlayerPower::InitCrease() {
	number_++;
	int last_pos;
	if (pos_list_.size() > 0) {
		last_pos = pos_list_.back();
		last_pos += 40;
	}
	else  {
		last_pos = 20;
	}
	pos_list_.push_back(last_pos);
	// xu ly them am thanh
}




void PlayerPower::Show(SDL_Renderer* screen) {
	for (int i = 0; i < pos_list_.size(); i++) {
		rect_.x = pos_list_.at(i);
		rect_.y = 0;
		Render(screen);
	}
}

PlayerMoney::PlayerMoney() {
	x_pos_ = 0;
	y_pos_ = 0;

}

PlayerMoney::~PlayerMoney() {

}

void PlayerMoney::Init(SDL_Renderer* screen) {
	bool ret = LoadImg("Debug//image//money_img.png", screen);

}

void PlayerMoney::Show(SDL_Renderer* screen) {
	rect_.x = x_pos_;
	rect_.y = y_pos_;
	Render(screen);
}