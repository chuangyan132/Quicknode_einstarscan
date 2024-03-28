#pragma once
#include <QObject>
#include <QVariantMap>

class HardwareBaseNode : public QObject {
    Q_OBJECT
  public:
    HardwareBaseNode(QObject *parent = nullptr){};
    virtual ~HardwareBaseNode(){};

  public slots:
    /*!
     * @brief 节点的功能函数
     */
    virtual void apply() = 0;

    /*!
     * @brief 设置节点的输入
     * @index 输入的索引
     * @value 输入的值
     */
    bool setInput(const QVariant &value) {
        QVariantMap map = value.toMap();
        for (auto iter = map.constBegin(); iter != map.constEnd(); iter++) {
            inputs[iter.key()] = iter.value();
        }
        return true;
    };

    /*!
     * @brief 设置节点的属性
     * @index 属性的索引
     * @value 属性的值
     */
    bool setProperty(const QVariant &value) {
        QVariantMap map = value.toMap();
        for (auto iter = map.constBegin(); iter != map.constEnd(); iter++) {
            properties[iter.key()] = iter.value();
        }
        return true;
    };

    /*!
     * @brief 获得节点的属性
     * @index 属性的端口名称
     */
    QVariant property(const QString &name) {
        return properties.value(name);
    };

    /*!
     * @brief 获得节点的输出
     * @index 输出的端口名称
     */
    QVariant output(const QString &name) {
        return outputs.value(name);
    };

    /*!
     * @brief 返回节点的输入/输出/属性参数/category描述信息
     * @return 返回形式为字符串形式
     */
    QString nodeDesc() {
        return m_descriptor;
    };

    /*!
     * @brief 返回节点的名称
     */
    QString nodeName() {
        return m_nodeName;
    }

    /*!
     * @brief 获得节点的输入/输出/属性参数/category描述信息
     * @return 返回形式为QList<QList<QStringList>>形式
     */
    QList<QList<QStringList>> description() {
        return m_description;
    };

  protected:
    bool hasInput(const QString &id) const {
        return inputs.contains(id) && !inputs.value(id).isNull();
    }

    QVariant input(const QString &id) const {
        return inputs.value(id);
    }

    void setOutput(const QString &id, const QVariant &value) {
        outputs[id] = value;
    }

  protected:
    //Q_DECLARE_PRIVATE(HardwareBaseNode)
    QVariantMap inputs;
    QVariantMap outputs;
    QVariantMap properties;
    QStringList categorys;
    QString m_descriptor;
    QString m_nodeName;
    QList<QList<QStringList>> m_description;
};

