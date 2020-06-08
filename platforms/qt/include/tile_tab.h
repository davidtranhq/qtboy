#ifndef TILE_TAB_H
#define TILE_TAB_H

#include <QWidget>
#include <QLabel>

#include "system.hpp"
#include "debugger.hpp"

class Qt_renderer;

class Tile_tab : public QWidget
{
    Q_OBJECT
    public:
    explicit Tile_tab(gameboy::System *s);

    private:
    gameboy::Debugger debugger_;
    Qt_renderer *renderer_;
    QLabel *bg_;


};

#endif // TILE_TAB_H
