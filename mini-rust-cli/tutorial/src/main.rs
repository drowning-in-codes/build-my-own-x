use bevy::window::WindowResizeConstraints;
use rand::prelude::*;
use rusty_engine::prelude::*;

#[derive(Resource)]
struct GameState {
    hight_score: i32,
    score: i32,
    enemy_labels: Vec<String>,
    spawn_timer: Timer,
    zombie_index: i32,
    velocity_delta: i32,
}
impl Default for GameState {
    fn default() -> Self {
        Self {
            hight_score: 0,
            score: 0,
            enemy_labels: vec![],
            spawn_timer: Timer::from_seconds(1.0, TimerMode::Repeating),
            zombie_index: 0,
            velocity_delta: 0,
        }
    }
}

fn main() {
    let mut game = Game::new();
    game.window_settings(Window {
        title: "Zombie Game".to_string(),
        resize_constraints: WindowResizeConstraints {
            min_width: 800.0,
            min_height: 600.0,
            max_width: 1800.0,
            max_height: 1600.0,
        },
        ..Default::default()
    });
    game.audio_manager.play_music(MusicPreset::Classy8Bit, 0.2);
    let player = game.add_sprite("player", SpritePreset::RacingCarBlue);
    player.translation = Vec2::new(10.0, 10.0);
    player.rotation = SOUTH_WEST;
    player.scale = 1.0;
    player.layer = 0.0;
    player.collision = true;

    let car1 = game.add_sprite("car1", SpritePreset::RacingCarRed);
    car1.translation = Vec2::new(20.0, 10.0);
    car1.layer = 1.1;
    car1.collision = true;

    let zombie_1 = game.add_sprite("zombie_original", "zombie_1.png");
    zombie_1.translation = Vec2::new(50.0, 310.0);
    zombie_1.layer = 1.1;
    zombie_1.collision = true;
    zombie_1.scale = 0.5;
    let score = game.add_text("score", "Score: 0");

    score.translation = Vec2::new(520.0, 280.0);

    let high_score = game.add_text("high_score", "High Score: 0");
    high_score.translation = Vec2::new(-520.0, 280.0);
    let fps = game.add_text("fps", "FPS: 0");
    fps.translation = Vec2::new(0.0, 280.0);

    game.add_logic(game_logic);
    game.run(GameState::default());
}

fn game_logic(engine: &mut Engine, game_state: &mut GameState) {
    // let text = format!("FPS: {}", (1.0 / engine.delta_f32));
    // engine.add_text("FPS", text);
    // engine.show_colliders = true;
    let offset = ((engine.time_since_startup_f64 * 2.0).cos() * 2.0) as f32;

    if engine.keyboard_state.just_pressed(KeyCode::Q) {
        engine.should_exit = true;
    }
    // key text near the edges of the screen
    let score = engine.texts.get_mut("score").unwrap();
    score.translation.x = engine.window_dimensions.x / 2.0 - 100.0;
    score.translation.y = engine.window_dimensions.y / 2.0 - 50.0 * offset;

    let high_score = engine.texts.get_mut("high_score").unwrap();
    high_score.translation.x = -engine.window_dimensions.x / 2.0 + 100.0;
    high_score.translation.y = engine.window_dimensions.y / 2.0 - 50.0;

    let fps = engine.texts.get_mut("fps").unwrap();
    fps.translation.y = engine.window_dimensions.y / 2.0 - 50.0;

    for event in engine.collision_events.drain(..) {
        if event.state == CollisionState::Begin && event.pair.one_starts_with("player") {
            game_state.score += 1;
            let score = engine.texts.get_mut("score").unwrap();
            score.value = format!("Score: {}", game_state.score);
            if game_state.score > game_state.hight_score {
                game_state.hight_score = game_state.score;
                let hight_score = engine.texts.get_mut("high_score").unwrap();
                hight_score.value = format!("High Score: {}", game_state.hight_score);
            }
            engine.audio_manager.play_sfx(SfxPreset::Confirmation1, 0.1);
            // remove the enemy car
            for label in [event.pair.0, event.pair.1] {
                if label != "player" {
                    engine.sprites.remove(&label);
                }
            }
        }
    }
    if game_state.spawn_timer.tick(engine.delta).just_finished() {
        let Vec2 {
            x: mut width,
            y: mut height,
        } = engine.window_dimensions;
        width = width / 2.0 - 20.0;
        height = height / 2.0 - 50.0;
        let label = format!("enemy_{}", game_state.zombie_index);
        let zombie = engine.add_sprite(label, "zombie_1.png");
        zombie.translation = Vec2::new(
            thread_rng().gen_range(-width..width),
            thread_rng().gen_range(-height..height),
        );
        zombie.scale = 0.5;
        zombie.collision = true;
        game_state.zombie_index += 1;
        game_state.velocity_delta += 10;
        let fps_label = engine.texts.get_mut("fps").unwrap();
        fps_label.value = format!("FPS: {}", (1.0 / engine.delta_f32) as i32);
    };
    let player = engine.sprites.get_mut("player").unwrap();
    // player.translation.x += 100.0 * engine.delta_f32;
    const MOVEMENT_SPEED: f32 = 100.0;
    let speed = MOVEMENT_SPEED + game_state.velocity_delta as f32;
    if engine
        .keyboard_state
        .pressed_any(&[KeyCode::Up, KeyCode::W])
    {
        player.translation.y += speed * engine.delta_f32;
    }
    if engine
        .keyboard_state
        .pressed_any(&[KeyCode::Left, KeyCode::A])
    {
        player.translation.x -= speed * engine.delta_f32;
    }
    if engine
        .keyboard_state
        .pressed_any(&[KeyCode::Right, KeyCode::D])
    {
        player.translation.x += speed * engine.delta_f32;
    }
    if engine
        .keyboard_state
        .pressed_any(&[KeyCode::Down, KeyCode::S])
    {
        player.translation.y -= speed * engine.delta_f32;
    }
    if engine.mouse_state.just_pressed(MouseButton::Left) {
        if let Some(mouse_location) = engine.mouse_state.location() {
            let zombie = engine.add_sprite(
                format!("zombie_{}", game_state.zombie_index),
                "zombie_1.png",
            );
            zombie.translation = mouse_location;
            zombie.layer = 1.1;
            zombie.collision = true;
            zombie.scale = 0.5;
            game_state.zombie_index += 1;
        }
    }
    // reset score
    if engine.keyboard_state.just_pressed(KeyCode::R) {
        game_state.score = 0;
        let score = engine.texts.get_mut("score").unwrap();
        score.value = format!("Score: {}", game_state.score);
        game_state.velocity_delta = 0;
    }
}
