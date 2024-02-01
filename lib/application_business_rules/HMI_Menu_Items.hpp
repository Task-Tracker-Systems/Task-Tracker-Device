#pragma once
#include "lvgl.h"
#include <string>
#include <vector>

namespace HMI
{
struct IScreen
{
    IScreen() = default;
    virtual ~IScreen() = default;

    virtual void draw() const = 0;

    static void exit();
};

enum class MenuItemType
{
    BUTTON,
    SWITCH,
};

struct IMenuItem
{
    IMenuItem() = default;
    virtual ~IMenuItem() = default;

    virtual std::string getText() const = 0;
    virtual MenuItemType getType() const = 0;
};
typedef std::vector<const IMenuItem *> MenuItemList;

struct MenuItemButton final : public IMenuItem
{
    MenuItemButton(std::string text, const IScreen *screenOnClick);
    ~MenuItemButton() override = default;

    std::string getText() const override;
    inline MenuItemType getType() const override
    {
        return MenuItemType::BUTTON;
    };

    const IScreen *getScreenOnClick() const;

  protected:
    const std::string _text;
    const IScreen *_screenOnClick;
};

struct MenuItemSwitch final : public IMenuItem
{
    MenuItemSwitch(std::string text, bool *ptrBool);
    ~MenuItemSwitch() override = default;

    std::string getText() const override;
    inline MenuItemType getType() const override
    {
        return MenuItemType::SWITCH;
    };

    bool *getPtrBool() const;

  protected:
    const std::string _text;
    bool *_ptrBool;
};

struct ScreenMenu final : public IScreen
{
    ScreenMenu() = default;
    ~ScreenMenu() override = default;

    void draw() const override;

    void addItem(const IMenuItem *newItem);

  private:
    MenuItemList _List;

    static void _button_cb(lv_event_t *e);
    static void _switch_cb(lv_event_t *e);
};

} // namespace HMI
