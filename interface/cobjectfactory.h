#ifndef COBJECT_FACTORY_HEARDER_H_
#define COBJECT_FACTORY_HEARDER_H_

class CObjectFactory
{

public:
    template<class T>
    void registerObject(const QString &className)
    {
        m_factoryObjects.insert(className, &(T::staticMetaObject));
    }

    template<class T>
    T* createObject(const QString &className, QObject *parent = 0)
    {
        const QMetaObject *meta = m_factoryObjects.value(className);

        //return (meta ?
         //           qobject_cast<T *>(meta->newInstance(Q_ARG(QObject *, parent)))
         //         : 0);
        return qobject_cast<T *>(meta->newInstance());
    }

private:
    QHash<QString, const QMetaObject *> m_factoryObjects;
};

#endif
