/*!
* @brief   
* @project qn_algorithm
* @file    basenode.h
* @author  zhaonanfang
* @email   zhaonanfang@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-zhaonanfang 2023-08-30 15:56
*/
//==============================================================================
#ifndef QN_ALGORITHM_BASENODE_H
#define QN_ALGORITHM_BASENODE_H
//==============================================================================


#include <QObject>
#include <QVariantMap>

class BaseNode : public QObject {
    Q_OBJECT
  public:
    BaseNode(QObject *parent = nullptr){};
    virtual ~BaseNode(){};

  public:
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
            m_inputs[iter.key()] = iter.value();
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
            m_properties[iter.key()] = iter.value();
        }
        return true;
    };

    /*!
     * @brief 获得节点的属性
     * @index 属性的端口名称
     */
    QVariant property(const QString &name) {
        return m_properties.value(name);
    };

    /*!
     * @brief 获得节点的输出
     * @index 输出的端口名称
     */
    QVariant output(const QString &name) {
        return m_outputs.value(name);
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

  protected:
    bool hasInput(const QString &id) const {
        return m_inputs.contains(id) && !m_inputs.value(id).isNull();
    }

    QVariant input(const QString &id) const {
        return m_inputs.value(id);
    }

    void setOutput(const QString &id, const QVariant &value) {
        m_outputs[id] = value;
    }

  protected:
    QVariantMap m_inputs;     // 输入
    QVariantMap m_outputs;    // 输出
    QVariantMap m_properties; // 属性
    QStringList m_categorys;  // category
    QString m_descriptor;   // 描述信息(预留)
    QString m_nodeName;     // 节点名称(预留)
};

//==============================================================================
#endif // QN_ALGORITHM_BASENODE_H
//==============================================================================
