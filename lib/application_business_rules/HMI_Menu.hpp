#pragma once

class menu
{
  public:
    menu() = default;
    virtual ~menu() = default;

    /// @brief build basic menu structure
    void initialize();

    /// @brief called cyclical for debug reasons
    void cyclic();
};
