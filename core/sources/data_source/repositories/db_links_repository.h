#ifndef LINK_MANAGER_H
#define LINK_MANAGER_H

// Internal
#include "i_links_repository.h"

namespace data_source
{
    class DbLinksRepository: public ILinksRepository
    {
        Q_OBJECT

    public:
        explicit DbLinksRepository(QObject* parent = nullptr);
        ~DbLinksRepository() override;

        dto::LinkDescriptionPtr description(int id) const override;
        dto::LinkDescriptionPtrList descriptions() const override;

        dto::LinkProtocolPtr protocol(int id) const override;
        dto::LinkProtocolPtrList protocols() const override;

        dto::LinkProtocolPtr protocolByName(const QString& name) const override;

    public slots:
        bool save(const dto::LinkDescriptionPtr& description) override;
        bool remove(const dto::LinkDescriptionPtr& description) override;

        bool save(const dto::LinkProtocolPtr& protocol) override;
        bool remove(const dto::LinkProtocolPtr& protocol) override;

    private:
        class Impl;
        QScopedPointer<Impl> const d;
    };
}

#endif // LINK_MANAGER_H
