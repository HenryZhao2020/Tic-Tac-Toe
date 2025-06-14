#pragma once

#include "Lang.h"

#include <QList>

/// The Tic-Tac-Toe board dimension (3x3)
constexpr int BOARD_DIM{3};

/**
 * @brief Represents the possible icons in a Tic-Tac-Toe square.
 */
enum class SquareIcon {
    NONE,   ///< The square is unmarked.
    X,      ///< The square is marked with an 'X'.
    O,      ///< The square is marked with an 'O'.
};

/**
 * @brief Holds shared game data for progress, settings, and statistics.
 */
class Attr {
public:
    /// Represents the current game progress.
    struct Progress {
        QList<SquareIcon> board{BOARD_DIM * BOARD_DIM,
                                SquareIcon::NONE};      ///< The board state.
        bool xTurn{stats.totalPlays() % 2 == 0};        ///< True if X's turn.
        bool ended{false};                              ///< True if game ended.
    };

    /// Stores user-defined settings.
    struct Settings {
        bool twoPlayer{false};      ///< True if two-player mode.
        bool animated{true};        ///< Enable animations.
        bool showScores{true};      ///< Show score counters.
        Lang lang{Lang::ENGLISH};   ///< Selected language.
    };

    /// Tracks player statistics.
    struct Stats {
        int xPoint{0};      ///< Wins by X.
        int oPoint{0};      ///< Wins by O.
        int tiePoint{0};    ///< Number of ties.

        int totalPlays() const {
            return xPoint + oPoint + tiePoint;
        }
    };

    /// Provides access to each section
    static Progress &getProgress();
    static Settings &getSettings();
    static Stats &getStats();

    /// Resets each section to default values.
    static void resetProgress();
    static void resetSettings();
    static void resetStats();

    /// Saves all attributes to disk.
    static void saveAttr();
    /// Loads all attributes from disk.
    /// Returns true if successful and false otherwise.
    static bool loadAttr();

private:
    static Progress progress;
    static Settings settings;
    static Stats stats;
};
