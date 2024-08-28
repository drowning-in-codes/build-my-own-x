use rand::prelude::*;
use rusty_engine::prelude::*;

#[derive(Resource)]
struct GameState {
    health_mouth: i16,
    lost: bool,
}
impl Default for GameState {
    fn default() -> Self {
        Self {
            health_mouth: 3,
            lost: false,
        }
    }
}
const PLAYER_SPEED: f32 = 250.0;
const ROAD_SPEED: f32 = 400.0;
fn main() {
    // game setup goes here
    let mut game = Game::new();
    let obstacle_presets = vec![
        SpritePreset::RacingBarrelRed,
        SpritePreset::RacingBarrelBlue,
        SpritePreset::RacingConeStraight,
    ];
    for (i, preset) in obstacle_presets.into_iter().enumerate() {
        let obstacle = game.add_sprite(format!("obstacle{}", i), preset);
        obstacle.collision = true;
        obstacle.layer = 15.0;
        obstacle.translation.x = thread_rng().gen_range(-500.0..600.0);
        obstacle.translation.y = thread_rng().gen_range(-300.0..300.0);
    }
    let message = game.add_text(
        "health",
        format!("Health: {}", GameState::default().health_mouth),
    );
    message.translation.y = 300.0;

    let player = game.add_sprite("player1", SpritePreset::RacingCarBlue);
    player.translation.x = -500.0;
    player.layer = 1.0;
    player.collision = true;

    for i in 0..10 {
        let roadline = game.add_sprite(format!("roadline{}", i), SpritePreset::RacingBarrierWhite);
        roadline.translation.x = -600.0 + 150.0 * i as f32;
        roadline.scale = 0.1;
    }
    game.audio_manager
        .play_music(MusicPreset::WhimsicalPopsicle, 0.2);

    game.add_logic(game_logic);
    game.run(GameState::default());
}

fn game_logic(engine: &mut Engine, game_state: &mut GameState) {
    if game_state.lost {
        return;
    }
    // game logic goes here
    for event in engine.collision_events.drain(..) {
        if !event.pair.either_contains("player") || event.state == CollisionState::End {
            continue;
        }
        if game_state.health_mouth > 0 {
            game_state.health_mouth -= 1;
            let message = engine.texts.get_mut("health").unwrap();
            message.value = format!("Health: {}", game_state.health_mouth);
            // engine.sprites.remove(&label);
            engine.audio_manager.play_sfx(SfxPreset::Impact3, 1.0);
        }
    }
    for obj in engine.sprites.values_mut() {
        if obj.label.starts_with("roadline") {
            obj.translation.x -= ROAD_SPEED * engine.delta_f32;
            if obj.translation.x < -675.0 {
                obj.translation.x += 1500.0;
            }
        }
        if obj.label.starts_with("obstacle") {
            obj.translation.x -= ROAD_SPEED * engine.delta_f32;
            if obj.translation.x < -600.0 {
                obj.translation.x = thread_rng().gen_range(600.0..1600.0);
                obj.translation.y = thread_rng().gen_range(-300.0..300.0);
            }
        }
    }

    let mut direction = 0.0;
    if engine.keyboard_state.pressed(KeyCode::Up) {
        direction += 1.0;
    }
    if engine.keyboard_state.pressed(KeyCode::Down) {
        direction -= 1.0;
    }
    let player = engine.sprites.get_mut("player1").unwrap();
    player.translation.y += PLAYER_SPEED * engine.delta_f32 * direction;
    player.rotation = direction * 0.15;
    if player.translation.y > 360.0 || player.translation.y < -360.0 {
        game_state.health_mouth = 0;
    }
    if game_state.health_mouth == 0 {
        game_state.lost = true;
        let game_over = engine.add_text("game_over", "Game Over");
        game_over.font_size = 125.0;
        engine.audio_manager.stop_music();
        engine.audio_manager.play_sfx(SfxPreset::Jingle1, 0.3);
    }
}
