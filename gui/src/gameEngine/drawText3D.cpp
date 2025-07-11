#include "GameEngine.hpp"

#define SHOW_LETTER_BOUNDARY false
#define LETTER_BOUNDARY_SIZE 0.01f
#define LETTER_BOUNDARY_COLOR RED

/**
 * @brief Draws a single Unicode codepoint (character) in 3D space using a given font.
 *
 * Renders the specified codepoint at the given 3D position, with support for font size, backface rendering,
 * and color tinting. Optionally draws a wireframe boundary for debugging.
 *
 * @param font The font to use for rendering the codepoint.
 * @param codepoint The Unicode codepoint to draw.
 * @param position The 3D position where the character will be drawn.
 * @param fontSize The size of the font in world units.
 * @param backface If true, draws the character's back face as well.
 * @param tint The color tint to apply to the character.
 */
void gui::GameEngine::DrawTextCodepoint3D(Font font, int codepoint, Vector3 position, float fontSize, bool backface, Color tint) {
  // Character index position in sprite font
  // NOTE: In case a codepoint is not available in the font, index returned points to '?'
  int index = GetGlyphIndex(font, codepoint);
  float scale = fontSize/(float)font.baseSize;

  // Character destination rectangle on screen
  // NOTE: We consider charsPadding on drawing
  position.x += (float)(font.glyphs[index].offsetX - font.glyphPadding)*scale;
  position.z += (float)(font.glyphs[index].offsetY - font.glyphPadding)*scale;

  // Character source rectangle from font texture atlas
  // NOTE: We consider chars padding when drawing, it could be required for outline/glow shader effects
  Rectangle srcRec = { font.recs[index].x - (float)font.glyphPadding, font.recs[index].y - (float)font.glyphPadding,
                       font.recs[index].width + 2.0f*font.glyphPadding, font.recs[index].height + 2.0f*font.glyphPadding };

  float width = (float)(font.recs[index].width + 2.0f*font.glyphPadding)*scale;
  float height = (float)(font.recs[index].height + 2.0f*font.glyphPadding)*scale;

  if (font.texture.id > 0)
  {
      const float x = 0.0f;
      const float y = 0.0f;
      const float z = 0.0f;

      // normalized texture coordinates of the glyph inside the font texture (0.0f -> 1.0f)
      const float tx = srcRec.x/font.texture.width;
      const float ty = srcRec.y/font.texture.height;
      const float tw = (srcRec.x+srcRec.width)/font.texture.width;
      const float th = (srcRec.y+srcRec.height)/font.texture.height;

      if (SHOW_LETTER_BOUNDARY) DrawCubeWiresV((Vector3){ position.x + width/2, position.y, position.z + height/2}, (Vector3){ width, LETTER_BOUNDARY_SIZE, height }, LETTER_BOUNDARY_COLOR);

      rlCheckRenderBatchLimit(4 + 4*backface);
      rlSetTexture(font.texture.id);

      rlPushMatrix();
          rlTranslatef(position.x, position.y, position.z);

          rlBegin(RL_QUADS);
              rlColor4ub(tint.r, tint.g, tint.b, tint.a);

              // Front Face
              rlNormal3f(0.0f, 1.0f, 0.0f);                                   // Normal Pointing Up
              rlTexCoord2f(tx, ty); rlVertex3f(x,         y, z);              // Top Left Of The Texture and Quad
              rlTexCoord2f(tx, th); rlVertex3f(x,         y, z + height);     // Bottom Left Of The Texture and Quad
              rlTexCoord2f(tw, th); rlVertex3f(x + width, y, z + height);     // Bottom Right Of The Texture and Quad
              rlTexCoord2f(tw, ty); rlVertex3f(x + width, y, z);              // Top Right Of The Texture and Quad

              if (backface)
              {
                  // Back Face
                  rlNormal3f(0.0f, -1.0f, 0.0f);                              // Normal Pointing Down
                  rlTexCoord2f(tx, ty); rlVertex3f(x,         y, z);          // Top Right Of The Texture and Quad
                  rlTexCoord2f(tw, ty); rlVertex3f(x + width, y, z);          // Top Left Of The Texture and Quad
                  rlTexCoord2f(tw, th); rlVertex3f(x + width, y, z + height); // Bottom Left Of The Texture and Quad
                  rlTexCoord2f(tx, th); rlVertex3f(x,         y, z + height); // Bottom Right Of The Texture and Quad
              }
          rlEnd();
      rlPopMatrix();

      rlSetTexture(0);
  }
}

/**
 * @brief Draws a UTF-8 encoded string in 3D space using a given font.
 *
 * Renders the specified text at the given 3D position, with support for font size, spacing, line spacing,
 * backface rendering, and color tinting. Handles newlines and advances position accordingly.
 *
 * @param font The font to use for rendering the text.
 * @param text The UTF-8 encoded string to draw.
 * @param position The 3D position where the text will start.
 * @param fontSize The size of the font in world units.
 * @param fontSpacing Additional spacing between characters.
 * @param lineSpacing Additional spacing between lines (on newline characters).
 * @param backface If true, draws the back face of each character as well.
 * @param tint The color tint to apply to the text.
 */
void gui::GameEngine::DrawText3D(Font font, const char *text, Vector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, Color tint) {
  int length = TextLength(text);          // Total length in bytes of the text, scanned by codepoints in loop

  float textOffsetY = 0.0f;               // Offset between lines (on line break '\n')
  float textOffsetX = 0.0f;               // Offset X to next character to draw

  float scale = fontSize/(float)font.baseSize;

  for (int i = 0; i < length;)
  {
      // Get next codepoint from byte string and glyph index in font
      int codepointByteCount = 0;
      int codepoint = GetCodepoint(&text[i], &codepointByteCount);
      int index = GetGlyphIndex(font, codepoint);

      // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
      // but we need to draw all of the bad bytes using the '?' symbol moving one byte
      if (codepoint == 0x3f) codepointByteCount = 1;

      if (codepoint == '\n')
      {
          // NOTE: Fixed line spacing of 1.5 line-height
          // TODO: Support custom line spacing defined by user
          textOffsetY += fontSize + lineSpacing;
          textOffsetX = 0.0f;
      }
      else
      {
          if ((codepoint != ' ') && (codepoint != '\t'))
          {
              DrawTextCodepoint3D(font, codepoint, (Vector3){ position.x + textOffsetX, position.y, position.z + textOffsetY }, fontSize, backface, tint);
          }

          if (font.glyphs[index].advanceX == 0) textOffsetX += (float)font.recs[index].width*scale + fontSpacing;
          else textOffsetX += (float)font.glyphs[index].advanceX*scale + fontSpacing;
      }

      i += codepointByteCount;   // Move text bytes counter to next codepoint
  }
}

