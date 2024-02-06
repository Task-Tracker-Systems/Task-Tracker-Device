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
    VALUE,
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

struct MenuItemValue final : public IMenuItem
{
    MenuItemValue(std::string text, double *ptrDouble, uint8_t decimals, double min, double max);
    ~MenuItemValue() override = default;

    std::string getText() const override;
    inline MenuItemType getType() const override
    {
        return MenuItemType::VALUE;
    };

    double *getPtrDouble() const;

    double *PtrDouble;
    uint8_t Decimals;
    double Min;
    double Max;

  protected:
    const std::string _text;
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
    static void _value_cb(lv_event_t *e);
};

struct ScreenValueModifier final : public IScreen
{
    ScreenValueModifier() = default;
    ~ScreenValueModifier() override = default;

    void draw() const override;

    void set(double *_ptrDouble, uint8_t _decimals, double _min, double _max);

  private:
    double *_ptrDouble;
    uint8_t _decimals;
    double _min;
    double _max;

    static void _button_inc_cb(lv_event_t *e);
    static void _button_dec_cb(lv_event_t *e);
    static void _button_step_cb(lv_event_t *e);
};

} // namespace HMI
