import irishgreencitrus.raylibv as r
fn main() {
	r.init_window(1000,1000,"bruh".str)
	r.set_target_fps(60)
	mut i := u64(0)
	for !r.window_should_close() {
		r.begin_drawing()
			r.clear_background(r.white)
			for x in 0..1000 {
				for y in 0..1000 {
					//r.draw_pixel(x,y,r.Color{u8(i/256),0,0,255})
					//r.draw_rectangle(x*10,y*10,10,10,r.Color{u8(v%256),0,0,255})
					if x%100==0 && y%100==0 {
						r.draw_line_v(r.Vector2{x,y},r.get_mouse_position(),r.Color{0,0,0,255})
					}
					
				}
			}
		r.end_drawing()
		r.draw_fps(10,10)
	}
	r.close_window()
}