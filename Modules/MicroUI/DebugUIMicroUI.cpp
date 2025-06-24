#include "DebugUIMicroUI.h"
#include "microui.h"

namespace hive::microui {
    void debug_ui_create_window() {}
    void debug_ui_update() {



        mu_begin(ctx);
        style_window(ctx);
        log_window(ctx);
        test_window(ctx);
        mu_end(ctx);
    }
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: exit(EXIT_SUCCESS); break;
                case SDL_MOUSEMOTION: mu_input_mousemove(ctx, e.motion.x, e.motion.y); break;
                case SDL_MOUSEWHEEL: mu_input_scroll(ctx, 0, e.wheel.y * -30); break;
                case SDL_TEXTINPUT: mu_input_text(ctx, e.text.text); break;

                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP: {
                    int b = button_map[e.button.button & 0xff];
                    if (b && e.type == SDL_MOUSEBUTTONDOWN) { mu_input_mousedown(ctx, e.button.x, e.button.y, b); }
                    if (b && e.type ==   SDL_MOUSEBUTTONUP) { mu_input_mouseup(ctx, e.button.x, e.button.y, b);   }
                    break;
                }

                case SDL_KEYDOWN:
                case SDL_KEYUP: {
                    int c = key_map[e.key.keysym.sym & 0xff];
                    if (c && e.type == SDL_KEYDOWN) { mu_input_keydown(ctx, c); }
                    if (c && e.type ==   SDL_KEYUP) { mu_input_keyup(ctx, c);   }
                    break;
                }
            }



            r_clear(mu_color(bg[0], bg[1], bg[2], 255));
            mu_Command *cmd = NULL;
            while (mu_next_command(ctx, &cmd)) {
                switch (cmd->type) {
                    case MU_COMMAND_TEXT: r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color); break;
                    case MU_COMMAND_RECT: r_draw_rect(cmd->rect.rect, cmd->rect.color); break;
                    case MU_COMMAND_ICON: r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color); break;
                    case MU_COMMAND_CLIP: r_set_clip_rect(cmd->clip.rect); break;
                }
            }
            r_present();

    }
    void debug_ui_create_button() {}
    void debug_ui_destroy_window() {}
}

