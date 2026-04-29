// Copyright 2010-2024 Benjamin ALBOUY-KISSI - Ported to Linux/SFML
/*
    This file is part of LibGraph.

    LibGraph is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LibGraph is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LibGraph. If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef LIBGRAPH2_USE_SFML
#define _USE_MATH_DEFINES
#include "LibGraph2impSFML.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

// Note: tinyfiledialogs sera ajouté plus tard
// #include "tinyfiledialogs.h"

using namespace std;
using namespace LibGraph2;

// Pointeur vers l'unique instance (singleton)
CLibGraph2 *CLibGraph2::s_pInstance = NULL;

// Fonction statique de récupération de l'instance
CLibGraph2 *CLibGraph2::GetInstance() {
  if (s_pInstance == NULL)
    s_pInstance = new CLibGraph2;
  return s_pInstance;
}

// Fonction statique de libération de l'instance
void CLibGraph2::ReleaseInstance() {
  delete s_pInstance;
  s_pInstance = NULL;
}

// Constructeur
CLibGraph2::CLibGraph2()
    : m_pWindow(NULL), m_outlineColor(sf::Color::Black),
      m_outlineThickness(1.0f), m_fillColor(sf::Color::Transparent),
      m_penStyle(pen_DashStyles::Solid), m_fontSize(10.0f),
      m_fontStyle(FontStyleRegular), m_nNormalisedSizeX(0),
      m_nNormalisedSizeY(0), m_dScale(1.0), m_nOffsetX(0), m_nOffsetY(0),
      m_bBackBuffered(false) {
  // Charger une police par défaut
  #ifdef _WIN32
  std::string defaultFont = "C:\\Windows\\Fonts\\arial.ttf";
#else
  std::string defaultFont = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
#endif
  FILE *f = fopen(defaultFont.c_str(), "r");
  if (f) {
    fclose(f);
    if (!m_font.loadFromFile(defaultFont))
      std::cerr << "Warning: SFML failed to load existing font: " << defaultFont
                << std::endl;
  } else {
    std::cerr << "Warning: Default font not found at " << defaultFont
              << std::endl;
  }

  // Initialiser les couleurs par défaut
  setPen(MakeARGB(255, 0, 0, 0), 1.0f);
  setSolidBrush(0);
}

// Destructeur
CLibGraph2::~CLibGraph2() {
  s_pInstance = NULL;
  if (m_pWindow) {
    m_pWindow->close();
    delete m_pWindow;
  }
}

// Fonctions privées
int CLibGraph2::getPixelWidth() {
  if (m_pWindow)
    return m_pWindow->getSize().x;
  return 0;
}

int CLibGraph2::getPixelHeight() {
  if (m_pWindow)
    return m_pWindow->getSize().y;
  return 0;
}

void CLibGraph2::ComputeScaleAndOffset() {
  if (m_nNormalisedSizeX == 0) {
    m_dScale = 1.0;
    m_nOffsetX = 0;
    m_nOffsetY = 0;
    return;
  }

  double dNormalisedRatio = (double)m_nNormalisedSizeX / m_nNormalisedSizeY;
  double dWindowRatio = (double)getPixelWidth() / getPixelHeight();

  if (dNormalisedRatio < dWindowRatio) {
    m_dScale = (double)getPixelHeight() / m_nNormalisedSizeY;
    m_nOffsetY = 0;
    m_nOffsetX = (int)((getPixelWidth() - m_dScale * m_nNormalisedSizeX) / 2);
  } else {
    m_dScale = (double)getPixelWidth() / m_nNormalisedSizeX;
    m_nOffsetX = 0;
    m_nOffsetY = (int)((getPixelHeight() - m_dScale * m_nNormalisedSizeY) / 2);
  }
}

bool CLibGraph2::NeedToFill() { return m_fillColor.a > 0; }

// Implémentation des fonctions publiques

void CLibGraph2::show(const CSize &szWndSize, bool bFullScreen) {
  int width = (int)szWndSize.m_fWidth;
  int height = (int)szWndSize.m_fHeight;

  if (szWndSize.m_fWidth * szWndSize.m_fHeight <= 1) {
    m_nNormalisedSizeX = 0;
    m_nNormalisedSizeY = 0;
    width = 800;
    height = 600;
  } else {
    m_nNormalisedSizeX = width;
    m_nNormalisedSizeY = height;
  }

  sf::Uint32 style = bFullScreen ? sf::Style::Fullscreen : sf::Style::Default;

  if (m_pWindow) {
    delete m_pWindow;
  }

  m_pWindow =
      new sf::RenderWindow(sf::VideoMode(width, height), LG_WINDOWTITLE, style);
  m_pWindow->setVerticalSyncEnabled(true);

  ComputeScaleAndOffset();
}

void CLibGraph2::hide() {
  if (m_pWindow)
    m_pWindow->setVisible(false);
}

CSize CLibGraph2::getSize() {
  CSize ret;

  if (m_nNormalisedSizeX != 0)
    ret.m_fWidth = (float)m_nNormalisedSizeX;
  else
    ret.m_fWidth = (float)getPixelWidth();

  if (m_nNormalisedSizeY != 0)
    ret.m_fHeight = (float)m_nNormalisedSizeY;
  else
    ret.m_fHeight = (float)getPixelHeight();

  return ret;
}

void CLibGraph2::showConsole() {
  // Sur Linux, la console est le terminal standard
  // Rien à faire ici
}

void CLibGraph2::hideConsole() {
  // Sur Linux, la console est le terminal standard
  // Rien à faire ici
}

void CLibGraph2::askForRefresh() {
  // Marquer qu'un rafraîchissement est nécessaire
  // Sera géré dans waitForEvent
}

void CLibGraph2::beginPaint() { m_bBackBuffered = true; }

void CLibGraph2::endPaint() {
  m_bBackBuffered = false;
  if (m_pWindow)
    m_pWindow->display();
}

// Fonctions de dessin

void CLibGraph2::setPen(ARGB color, float fWidth, pen_DashStyles style) {
  m_outlineColor =
      sf::Color(GetR(color), GetG(color), GetB(color), GetA(color));
  m_outlineThickness = fWidth * m_dScale;
  m_penStyle = style;
  // Note: SFML ne supporte pas nativement les styles pointillés
}

void CLibGraph2::setSolidBrush(ARGB color) {
  m_fillColor = sf::Color(GetR(color), GetG(color), GetB(color), GetA(color));
}

void CLibGraph2::setTextureBrush(const CString &sFileName) {
  // TODO: Implémenter le brush texturé avec SFML
  // Nécessite de charger une texture et l'appliquer aux formes
}

void CLibGraph2::drawLine(const CPoint &ptP1, const CPoint &ptP2) {
  if (!m_pWindow)
    return;

  sf::VertexArray line(sf::Lines, 2);

  line[0].position =
      sf::Vector2f(UnmapCoordinateX(ptP1.m_fX), UnmapCoordinateY(ptP1.m_fY));
  line[0].color = m_outlineColor;

  line[1].position =
      sf::Vector2f(UnmapCoordinateX(ptP2.m_fX), UnmapCoordinateY(ptP2.m_fY));
  line[1].color = m_outlineColor;

  m_pWindow->draw(line);

  if (!m_bBackBuffered)
    m_pWindow->display();
}

void CLibGraph2::drawRectangle(const CRectangle &bounds) {
  if (!m_pWindow)
    return;

  sf::RectangleShape rect;

  rect.setSize(sf::Vector2f(UnmapWidth(bounds.m_szSize.m_fWidth),
                            UnmapHeight(bounds.m_szSize.m_fHeight)));
  rect.setPosition(UnmapCoordinateX(bounds.m_ptTopLeft.m_fX),
                   UnmapCoordinateY(bounds.m_ptTopLeft.m_fY));

  rect.setFillColor(m_fillColor);
  rect.setOutlineColor(m_outlineColor);
  rect.setOutlineThickness(m_outlineThickness);

  m_pWindow->draw(rect);

  if (!m_bBackBuffered)
    m_pWindow->display();
}

void CLibGraph2::drawEllipse(const CRectangle &bounds) {
  if (!m_pWindow)
    return;

  sf::CircleShape ellipse;

  float radiusX = UnmapWidth(bounds.m_szSize.m_fWidth) / 2.0f;
  float radiusY = UnmapHeight(bounds.m_szSize.m_fHeight) / 2.0f;

  ellipse.setRadius(radiusX);
  ellipse.setScale(1.0f, radiusY / radiusX);
  ellipse.setPosition(UnmapCoordinateX(bounds.m_ptTopLeft.m_fX),
                      UnmapCoordinateY(bounds.m_ptTopLeft.m_fY));

  ellipse.setFillColor(m_fillColor);
  ellipse.setOutlineColor(m_outlineColor);
  ellipse.setOutlineThickness(m_outlineThickness);

  m_pWindow->draw(ellipse);

  if (!m_bBackBuffered)
    m_pWindow->display();
}

void CLibGraph2::drawArc(const CRectangle &rectBounds, float startAngle,
                         float sweepAngle) {
  // TODO: Implémenter drawArc avec SFML
  // Similaire à drawPie mais sans remplissage et sans lignes vers le centre
}

void CLibGraph2::drawPie(const CRectangle &rectBounds, float startAngle,
                         float sweepAngle) {
  drawPieInternal(rectBounds, startAngle, sweepAngle);

  if (!m_bBackBuffered && m_pWindow)
    m_pWindow->display();
}

void CLibGraph2::drawPieInternal(const CRectangle &bounds, float startAngle,
                                 float sweepAngle) {
  if (!m_pWindow)
    return;

  const int segments = 50;
  sf::VertexArray pie(sf::TriangleFan, segments + 2);

  float centerX = UnmapCoordinateX(bounds.m_ptTopLeft.m_fX +
                                   bounds.m_szSize.m_fWidth / 2.0f);
  float centerY = UnmapCoordinateY(bounds.m_ptTopLeft.m_fY +
                                   bounds.m_szSize.m_fHeight / 2.0f);
  float radiusX = UnmapWidth(bounds.m_szSize.m_fWidth) / 2.0f;
  float radiusY = UnmapHeight(bounds.m_szSize.m_fHeight) / 2.0f;

  // Premier point = centre
  pie[0].position = sf::Vector2f(centerX, centerY);
  pie[0].color = m_fillColor;

  // Points sur l'arc
  float startRad = startAngle * M_PI / 180.0f;
  float sweepRad = sweepAngle * M_PI / 180.0f;

  for (int i = 0; i <= segments; i++) {
    float angle = startRad + (sweepRad * i / segments);
    float x = centerX + radiusX * cos(angle);
    float y = centerY + radiusY * sin(angle);

    pie[i + 1].position = sf::Vector2f(x, y);
    pie[i + 1].color = m_fillColor;
  }

  m_pWindow->draw(pie);
}

void CLibGraph2::drawPolylines(const vector<CPoint> &vPoints, bool bAutoClose) {
  if (!m_pWindow)
    return;

  if (bAutoClose) {
    // Polygone fermé
    sf::ConvexShape polygon;
    polygon.setPointCount(vPoints.size());

    for (size_t i = 0; i < vPoints.size(); i++) {
      polygon.setPoint(i, sf::Vector2f(UnmapCoordinateX(vPoints[i].m_fX),
                                       UnmapCoordinateY(vPoints[i].m_fY)));
    }

    polygon.setFillColor(m_fillColor);
    polygon.setOutlineColor(m_outlineColor);
    polygon.setOutlineThickness(m_outlineThickness);

    m_pWindow->draw(polygon);
  } else {
    // Ligne brisée
    sf::VertexArray lines(sf::LineStrip, vPoints.size());

    for (size_t i = 0; i < vPoints.size(); i++) {
      lines[i].position = sf::Vector2f(UnmapCoordinateX(vPoints[i].m_fX),
                                       UnmapCoordinateY(vPoints[i].m_fY));
      lines[i].color = m_outlineColor;
    }

    m_pWindow->draw(lines);
  }

  if (!m_bBackBuffered)
    m_pWindow->display();
}

void CLibGraph2::setPixel(const CPoint &ptPos, ARGB color) {
  if (!m_pWindow)
    return;

  sf::RectangleShape pixel;
  pixel.setSize(sf::Vector2f(1, 1));
  pixel.setPosition(UnmapCoordinateX(ptPos.m_fX), UnmapCoordinateY(ptPos.m_fY));
  pixel.setFillColor(
      sf::Color(GetR(color), GetG(color), GetB(color), GetA(color)));

  m_pWindow->draw(pixel);

  if (!m_bBackBuffered)
    m_pWindow->display();
}

void CLibGraph2::setFont(const CString &strFontName, float fPointSize,
                         font_styles nStyleFlags) {
  m_fontSize = fPointSize * m_dScale;
  m_fontStyle = nStyleFlags;

  // Essayer de charger la police depuis le système
  std::string fontName = std::string(strFontName);
  #ifdef _WIN32
  std::vector<std::string> paths = {
      "C:\\Windows\\Fonts\\" + fontName + ".ttf",
      "C:\\Windows\\Fonts\\arial.ttf"
  };
#else
  std::vector<std::string> paths = {
      "/usr/share/fonts/truetype/" + fontName + ".ttf",
      "/usr/share/fonts/truetype/dejavu/" + fontName + ".ttf",
      "/usr/share/fonts/truetype/liberation/" + fontName + ".ttf",
      "/usr/share/fonts/truetype/freefont/" + fontName + ".ttf"};
#endif

  bool loaded = false;
  for (const auto &path : paths) {
    FILE *f = fopen(path.c_str(), "r");
    if (f) {
      fclose(f);
      if (m_font.loadFromFile(path)) {
        loaded = true;
        break;
      }
    }
  }

  if (!loaded) {
    // Fallback silencieux vers DejaVuSans si déjà chargé ou tenter le chemin
    // dur
    #ifdef _WIN32
    m_font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
#else
    m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
#endif
  }
}

void CLibGraph2::drawString(const CString &text, const CPoint &ptPos) {
  if (!m_pWindow)
    return;

  sf::Text sfText;
  sfText.setFont(m_font);
  sfText.setString(std::wstring(text));
  sfText.setCharacterSize(static_cast<unsigned int>(m_fontSize));
  sfText.setFillColor(m_fillColor);
  sfText.setPosition(UnmapCoordinateX(ptPos.m_fX),
                     UnmapCoordinateY(ptPos.m_fY));

  // Appliquer le style
  sf::Uint32 style = sf::Text::Regular;
  if (m_fontStyle & FontStyleBold)
    style |= sf::Text::Bold;
  if (m_fontStyle & FontStyleItalic)
    style |= sf::Text::Italic;
  sfText.setStyle(style);

  m_pWindow->draw(sfText);

  if (!m_bBackBuffered)
    m_pWindow->display();
}

void CLibGraph2::getStringDimension(const CString &text, const CPoint &ptPos,
                                    CRectangle &rectBounds) {
  sf::Text sfText;
  sfText.setFont(m_font);
  sfText.setString(std::wstring(text));
  sfText.setCharacterSize(static_cast<unsigned int>(m_fontSize));

  sf::FloatRect bounds = sfText.getLocalBounds();

  rectBounds.m_ptTopLeft.m_fX = MapCoordinateX(bounds.left);
  rectBounds.m_ptTopLeft.m_fY = MapCoordinateY(bounds.top);
  rectBounds.m_szSize.m_fWidth = MapWidth(bounds.width);
  rectBounds.m_szSize.m_fHeight = MapHeight(bounds.height);
}

void CLibGraph2::drawBitmap(const CString &sFileName, const CPoint &ptPos,
                            double dScaleFactor, double dAngleDeg,
                            bool bXYIsCenter) {
  if (!m_pWindow)
    return;

  std::string filename = std::string(sFileName);

  // Charger ou récupérer la texture du cache
  if (m_textureCache.find(filename) == m_textureCache.end()) {
    sf::Texture texture;
    if (!texture.loadFromFile(filename))
      return; // Erreur de chargement
    m_textureCache[filename] = texture;
  }

  sf::Sprite sprite;
  sprite.setTexture(m_textureCache[filename]);

  // Transformations
  if (bXYIsCenter) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
  }

  sprite.setPosition(UnmapCoordinateX(ptPos.m_fX),
                     UnmapCoordinateY(ptPos.m_fY));
  sprite.setScale(dScaleFactor * m_dScale, dScaleFactor * m_dScale);
  sprite.setRotation(dAngleDeg);

  m_pWindow->draw(sprite);

  if (!m_bBackBuffered)
    m_pWindow->display();
}

void CLibGraph2::drawBitmap(const CString &sFileName, const CPoint &ptPos,
                            const CPoint &ptPosPivot, double dScaleFactor,
                            double dAngleDeg) {
  if (!m_pWindow)
    return;

  std::string filename = std::string(sFileName);

  if (m_textureCache.find(filename) == m_textureCache.end()) {
    sf::Texture texture;
    if (!texture.loadFromFile(filename))
      return;
    m_textureCache[filename] = texture;
  }

  sf::Sprite sprite;
  sprite.setTexture(m_textureCache[filename]);

  sprite.setOrigin(ptPosPivot.m_fX, ptPosPivot.m_fY);
  sprite.setPosition(UnmapCoordinateX(ptPos.m_fX),
                     UnmapCoordinateY(ptPos.m_fY));
  sprite.setScale(dScaleFactor * m_dScale, dScaleFactor * m_dScale);
  sprite.setRotation(dAngleDeg);

  m_pWindow->draw(sprite);

  if (!m_bBackBuffered)
    m_pWindow->display();
}

// Gestion des événements

// Traducteur SFML Key -> Windows Virtual Key / ASCII
static unsigned int MapSFMLKeyToWinVK(sf::Keyboard::Key key) {
  // Lettres A-Z -> ASCII 65-90
  if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
    return 65 + (key - sf::Keyboard::A);

  // Chiffres 0-9 (haut du clavier) -> ASCII 48-57
  if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9)
    return 48 + (key - sf::Keyboard::Num0);

  // Pavé numérique 0-9 -> codes VK 96-105
  if (key >= sf::Keyboard::Numpad0 && key <= sf::Keyboard::Numpad9)
    return 96 + (key - sf::Keyboard::Numpad0);

  // Symboles et touches spéciales selon la table ASCII et les Virtual Keys
  switch (key) {
  // Ponctuation et symboles ASCII
  case sf::Keyboard::Space:
    return 32; // Espace
  case sf::Keyboard::Comma:
    return 44; // ,
  case sf::Keyboard::Period:
    return 46; // .
  case sf::Keyboard::Slash:
    return 47; // /
  case sf::Keyboard::SemiColon:
    return 59; // ;
  case sf::Keyboard::Equal:
    return 61; // =
  case sf::Keyboard::Dash:
    return 45; // -
  case sf::Keyboard::LBracket:
    return 91; // [
  case sf::Keyboard::RBracket:
    return 93; // ]
  case sf::Keyboard::BackSlash:
    return 92; // \
  case sf::Keyboard::Quote:     return 39;  // '
  case sf::Keyboard::Tilde:
    return 126; // ~

  // Pavé numérique (Opérateurs ASCII)
  case sf::Keyboard::Add:
    return 43; // +
  case sf::Keyboard::Subtract:
    return 45; // -
  case sf::Keyboard::Multiply:
    return 42; // *
  case sf::Keyboard::Divide:
    return 47; // /

  // Touches de contrôle (Codes Windows Virtual Key / ASCII)
  case sf::Keyboard::Escape:
    return 27; // ESC (ASCII 27)
  case sf::Keyboard::Enter:
    return 13; // CR / Entrée (ASCII 13)
  case sf::Keyboard::BackSpace:
    return 8; // BS (ASCII 8)
  case sf::Keyboard::Tab:
    return 9; // TAB (ASCII 9)

  case sf::Keyboard::LShift:
  case sf::Keyboard::RShift:
    return 16; // VK_SHIFT
  case sf::Keyboard::LControl:
  case sf::Keyboard::RControl:
    return 17; // VK_CONTROL
  case sf::Keyboard::LAlt:
  case sf::Keyboard::RAlt:
    return 18; // VK_MENU (ALT)
  case sf::Keyboard::Pause:
    return 19; // PAUSE

  case sf::Keyboard::Left:
    return 37; // VK_LEFT
  case sf::Keyboard::Up:
    return 38; // VK_UP
  case sf::Keyboard::Right:
    return 39; // VK_RIGHT
  case sf::Keyboard::Down:
    return 40; // VK_DOWN

  case sf::Keyboard::Insert:
    return 45; // VK_INSERT
  case sf::Keyboard::Delete:
    return 46; // VK_DELETE
  case sf::Keyboard::PageUp:
    return 33; // VK_PRIOR
  case sf::Keyboard::PageDown:
    return 34; // VK_NEXT
  case sf::Keyboard::End:
    return 35; // VK_END
  case sf::Keyboard::Home:
    return 36; // VK_HOME

  default:
    return 0;
  }
}

bool CLibGraph2::waitForEvent(evt &e) {
  if (!m_pWindow)
    return false;

  sf::Event event;

  while (m_pWindow->pollEvent(event)) {
    switch (event.type) {
    case sf::Event::MouseMoved:
      e.type = evt_type::evtMouseMove;
      e.x = (unsigned int)MapCoordinateX((float)event.mouseMove.x);
      e.y = (unsigned int)MapCoordinateY((float)event.mouseMove.y);
      m_lastEvent = e;
      return true;

    case sf::Event::MouseButtonPressed:
      e.type = evt_type::evtMouseDown;
      e.x = (unsigned int)MapCoordinateX((float)event.mouseButton.x);
      e.y = (unsigned int)MapCoordinateY((float)event.mouseButton.y);
      m_lastEvent = e;
      return true;

    case sf::Event::MouseButtonReleased:
      e.type = evt_type::evtMouseUp;
      e.x = (unsigned int)MapCoordinateX((float)event.mouseButton.x);
      e.y = (unsigned int)MapCoordinateY((float)event.mouseButton.y);
      m_lastEvent = e;
      return true;

    case sf::Event::KeyPressed:
      e.type = evt_type::evtKeyDown;
      // Conversion des codes de touches SFML vers codes Windows-like (ASCII
      // pour lettres/chiffres)
      e.vkKeyCode = MapSFMLKeyToWinVK(event.key.code);
      m_lastEvent = e;
      return true;

    case sf::Event::KeyReleased:
      e.type = evt_type::evtKeyUp;
      e.vkKeyCode = MapSFMLKeyToWinVK(event.key.code);
      m_lastEvent = e;
      return true;

    case sf::Event::Resized:
      e.type = evt_type::evtSize;
      e.x = event.size.width;
      e.y = event.size.height;
      ComputeScaleAndOffset();
      m_lastEvent = e;
      return true;

    case sf::Event::Closed:
      e.type = evt_type::evtClose;
      m_lastEvent = e;
      return false;

    default:
      break;
    }
  }

  // Générer un événement de rafraîchissement
  if (m_pWindow->isOpen()) {
    m_pWindow->clear(sf::Color::White);
    e.type = evt_type::evtRefresh;
    m_lastEvent = e;
    return true;
  }

  return false;
}

// Boîtes de dialogue - Implémentations temporaires
// TODO: Intégrer tinyfiledialogs

bool CLibGraph2::guiGetFileName(CString &sFileName, bool bOpen,
                                const vector<CString> &vstrFileTypes) {
  // TODO: Implémenter avec tinyfiledialogs
  std::cerr << "guiGetFileName not yet implemented" << std::endl;
  return false;
}

bool CLibGraph2::guiGetColor(ARGB &color) {
  // TODO: Implémenter avec tinyfiledialogs
  std::cerr << "guiGetColor not yet implemented" << std::endl;
  return false;
}

bool CLibGraph2::guiGetPenStyle(ARGB &color, float &fWidth,
                                pen_DashStyles &style) {
  // TODO: Implémenter avec tinyfiledialogs
  std::cerr << "guiGetPenStyle not yet implemented" << std::endl;
  return false;
}

bool CLibGraph2::guiGetFont(CString &strFontName, float &fPointSize,
                            font_styles &nStyleFlags) {
  // TODO: Implémenter avec tinyfiledialogs
  std::cerr << "guiGetFont not yet implemented" << std::endl;
  return false;
}

bool CLibGraph2::guiGetValue(CString &strVal, const CString &strTitle,
                             const CString &strLabel) {
  // TODO: Implémenter avec tinyfiledialogs
  std::cerr << "guiGetValue not yet implemented" << std::endl;
  return false;
}

bool CLibGraph2::guiGetValue(int &nVal, const CString &strTitle,
                             const CString &strLabel) {
  // TODO: Implémenter avec tinyfiledialogs
  std::cerr << "guiGetValue(int) not yet implemented" << std::endl;
  return false;
}

bool CLibGraph2::guiGetValue(double &dVal, const CString &strTitle,
                             const CString &strLabel) {
  // TODO: Implémenter avec tinyfiledialogs
  std::cerr << "guiGetValue(double) not yet implemented" << std::endl;
  return false;
}

msgbtn_answer CLibGraph2::guiMessageBox(const CString &strTitle,
                                        const CString &strText,
                                        msgbtn_types btns, msgicon_types icon,
                                        msgdefbtn_vals defbtn) {
  // TODO: Implémenter avec tinyfiledialogs
  std::cerr << "guiMessageBox not yet implemented" << std::endl;
  return msgbtn_answer::MsgAnsOk;
}

#endif // LIBGRAPH2_USE_SFML
