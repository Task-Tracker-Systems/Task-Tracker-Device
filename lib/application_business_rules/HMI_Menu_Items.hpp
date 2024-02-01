#pragma once
#include <vector>
#include <string>

namespace HMI
{
struct IScreen
{
    IScreen() = default;
    virtual ~IScreen() = default;

    virtual void draw() const = 0;
    
    void exit();
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
    inline MenuItemType getType() const override { return MenuItemType::BUTTON; };

  protected:
    const std::string _text;
    const IScreen *_screenOnClick;
};

struct MenuItemSwitch final : public IMenuItem
{
    MenuItemSwitch(std::string text, bool *varialeAddress);
    ~MenuItemSwitch() override = default;

    std::string getText() const override;
    inline MenuItemType getType() const override { return MenuItemType::SWITCH; };

  protected:
    const std::string _text;
    bool *_varialeAddress;
};

struct ScreenMenu final : public IScreen
{
    ScreenMenu() = default;
    ~ScreenMenu() override = default;

    void draw() const override;
    void addItem(const IMenuItem *newItem);

  private:
    MenuItemList _List;
};

} // namespace HMI
