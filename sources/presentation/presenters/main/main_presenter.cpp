#include "main_presenter.h"

// Qt
#include <QDebug>

// Internal
#include "topbar_presenter.h"

#include "map_handle.h"
#include "location_map_presenter.h"

#include "active_video_presenter.h"
#include "menu_presenter.h"

using namespace presentation;

class MainPresenter::Impl
{
public:
    TopbarPresenter* topbar;
    LocationMapPresenter* map;
    ActiveVideoPresenter* video;
    MenuPresenter* menu;
};

MainPresenter::MainPresenter(QObject* parent):
    BasePresenter(parent),
    d(new Impl())
{
    d->topbar = new TopbarPresenter(this);
    d->map = new  LocationMapPresenter(this);
    d->video = new ActiveVideoPresenter(this);
    d->menu = new MenuPresenter(d->map->handle(), this);
}

MainPresenter::~MainPresenter()
{}

void MainPresenter::connectView(QObject* view)
{
    d->topbar->setView(view->findChild<QObject*>(NAME(topbar)));
    d->menu->setView(view->findChild<QObject*>(NAME(menu)));

    connect(d->map->handle(), SIGNAL(reloadMap()), view, SLOT(reloadMap()));

    d->map->setView(this->view()->findChild<QObject*>("map"));
    connect(view, SIGNAL(requestPresenter(QString, QObject*)),
            this, SLOT(onRequestPresenter(QString, QObject*)));
}

void MainPresenter::onRequestPresenter(const QString& name, QObject* view)
{
    if (name == "map") d->map->setView(view);
    else if (name == "video") d->video->setView(view);
}