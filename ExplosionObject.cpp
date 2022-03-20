#include"ExplosionObject.h"

ExplosionObject::ExplosionObject() {
	frame_width_ = 0;
	frame_height_ = 0;
	frame_ = 0;
}

ExplosionObject::~ExplosionObject() {

}

bool ExplosionObject::LoadImg(std::string path, SDL_Renderer* screen) {
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret) {
		frame_width_ = rect_.w / NUM_FRAME_EXP;
		frame_height_ = rect_.h;
	}
	return ret;
}

void ExplosionObject::set_clip() {
	if (frame_width_ > 0 && frame_height_ > 0) {
		frame_clip_[0].x = 0;
		frame_clip_[0].y = 0;
		frame_clip_[0].w = frame_width_;
		frame_clip_[0].h = frame_height_;

		frame_clip_[1].x = frame_width_;
		frame_clip_[1].y = 0;
		frame_clip_[1].w = frame_width_;
		frame_clip_[1].h = frame_height_;


		frame_clip_[2].x = 2 * frame_width_;
		frame_clip_[2].y = 0;
		frame_clip_[2].w = frame_width_;
		frame_clip_[2].h = frame_height_;

		frame_clip_[3].x = 3 * frame_width_;
		frame_clip_[3].y = 0;
		frame_clip_[3].w = frame_width_;
		frame_clip_[3].h = frame_height_;

		frame_clip_[4].x = 4 * frame_width_;
		frame_clip_[4].y = 0;
		frame_clip_[4].w = frame_width_;
		frame_clip_[4].h = frame_height_;

		frame_clip_[5].x = 5 * frame_width_;
		frame_clip_[5].y = 0;
		frame_clip_[5].w = frame_width_;
		frame_clip_[5].h = frame_height_;

		frame_clip_[6].x = 6 * frame_width_;
		frame_clip_[6].y = 0;
		frame_clip_[6].w = frame_width_;
		frame_clip_[6].h = frame_height_;

		frame_clip_[7].x = 7 * frame_width_;
		frame_clip_[7].y = 0;
		frame_clip_[7].w = frame_width_;
		frame_clip_[7].h = frame_height_;
	}
}

void ExplosionObject::Show(SDL_Renderer* screen) {
	SDL_Rect* current_clip = &frame_clip_[frame_];
	SDL_Rect render_quad = { rect_.x,rect_.y,frame_width_,frame_height_ };
	if (current_clip != NULL) {
		render_quad.w = current_clip->w;
		render_quad.h = current_clip->h;
	}

	SDL_RenderCopy(screen, p_object_, current_clip, &render_quad);
}

void ExplosionObject::start_explosion(SDL_Rect object1,SDL_Renderer* screen,ExplosionObject &explosion) {
		for (int ex = 0; ex < NUM_FRAME_EXP; ++ex) {
			int x_pos = object1.x - explosion.get_frame_width() * 0.3;
			int y_pos = object1.y - explosion.get_frame_height() * 0.3;
			explosion.set_frame(ex);
			explosion.SetRect(x_pos,y_pos);
			explosion.Show(screen);
		}
	}
