#include "presenter_factory_interface.hpp"
#include "Menu.hpp"
#include "Presenter.hpp"
#include "display_factory_interface.hpp"
#include "statusindicators_factory_interface.hpp"

namespace hmi
{
IPresenter &getPresenter()
{
    static Menu singleMenu(board::getDisplay());
    static Presenter singleton(singleMenu, board::getStatusIndicators());
    return singleton;
}
} // namespace hmi