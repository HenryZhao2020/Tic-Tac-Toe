#pragma once

#include <QWidget>
#include <QTimer>

/**
 * @brief A TextWidget is any QWidget with text-capable member functions.
 * @example A TextWidget supports `void setText(const QString &text)`.
 */
template<typename T>
concept TextWidget =
    std::derived_from<T, QWidget> &&
    requires(T t, const QString &text) {
        { t.setText(text) } -> std::same_as<void>;
    };

/**
 * @brief An IconWidget is any QWidget with icon-capable member functions.
 * @example An IconWidget supports `void setIcon(const QIcon &icon)`.
 */
template<typename T>
concept IconWidget =
    std::derived_from<T, QWidget> &&
    requires(T t, const QIcon &icon) {
        { t.setIcon(icon) } -> std::same_as<void>;
    };

/**
 * @brief Collection of animation for text-capable widgets.
 */
class TextAnimation {
public:
    template<TextWidget T>
    explicit TextAnimation(T *t) : widget{t},
        textFunc{[t]() { return t->text(); }},
        setTextFunc{[t](const QString &text) { t->setText(text); }} {}
    ~TextAnimation();

    void typewrite(const QString &text, int interval = 20);

private:
    QWidget *widget{nullptr};
    QTimer *typewriteTimer{nullptr};

    const std::function<QString()> textFunc;
    const std::function<void(const QString &)> setTextFunc;
};

/**
 * @brief Collection of animation for icon-capable widgets.
 */
class IconAnimation {
public:
    template<IconWidget T>
    explicit IconAnimation(T *t) : widget{t},
        iconFunc{[t]() { return t->icon(); }},
        setIconFunc{[t](const QIcon &icon) { t->setIcon(icon); }},
        iconSizeFunc{[t]() { return t->iconSize(); }},
        setIconSizeFunc{[t](const QSize &size) { t->setIconSize(size); }} {}
    ~IconAnimation();

    void zoomIn(int finalSize, int interval = 10);
    void flash(int flashes = 2, int interval = 250);

private:
    QWidget *widget{nullptr};
    QTimer *zoomInTimer{nullptr};
    QTimer *flashTimer{nullptr};

    const std::function<QIcon()> iconFunc;
    const std::function<void(const QIcon &)> setIconFunc;
    const std::function<QSize()> iconSizeFunc;
    const std::function<void(const QSize &)> setIconSizeFunc;
};
