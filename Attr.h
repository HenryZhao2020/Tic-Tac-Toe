#pragma once

#include <QList>

/**
 * @brief Represents the possible icons in a Tic-Tac-Toe square.
 *
 * Used to indicate whether a square is marked with 'X', 'O', or is empty.
 */
enum class SquareIcon {
    X,     ///< The square is marked with an 'X'.
    O,     ///< The square is marked with an 'O'.
    EMPTY  ///< The square is unmarked (empty).
};

/**
 * @brief Provides language names and their corresponding locale information.
 *
 * Used for multi-language support in the UI.
 */
class Lang {
public:
    /// Supported language identifiers.
    enum Name {
        ENGLISH,
        CHINESE_SIMP,
        CHINESE_TRAD,
    };

    /// Returns a list of localized language names for UI display.
    static const QStringList &getLangNames();

    /// Returns the locale territory code (e.g., "en_US", "zh_CN") for the given language.
    static QString getLangTerrCode(const Name &name);
};

/**
 * @brief Holds shared game data for progress, settings, and statistics.
 *
 * Provides access and control functions for saving, loading, and resetting.
 */
class Attr {
public:
    /// Represents the current game progress.
    struct Progress {
        QList<SquareIcon> board {9, SquareIcon::EMPTY}; ///< The 3x3 board state.
        bool xTurn {true};                              ///< True if X's turn.
        bool ended {false};                             ///< True if game ended.
    };

    /// Stores user-defined settings.
    struct Settings {
        Lang::Name lang {Lang::ENGLISH};  ///< Selected language.
        bool animated {true};             ///< Enable animations.
        bool twoPlayer {false};           ///< True if two-player mode.
        bool showScores {true};           ///< Show score counters.
    };

    /// Tracks player statistics.
    struct Stats {
        int xPoint {0};     ///< Wins by X.
        int oPoint {0};     ///< Wins by O.
        int tiePoint {0};   ///< Number of ties.

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
