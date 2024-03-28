/*!
* @brief   
* @project exepluginexample_test_1
* @file    basenode.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-07-25 20:03
*/
//==============================================================================
#ifndef EXEPLUGINEXAMPLE_TEST_1_BASENODE_H
#define EXEPLUGINEXAMPLE_TEST_1_BASENODE_H
//==============================================================================

#include <string>
#include <jsoncpp/json/json-forwards.h>
#include <jsoncpp/json/json.h>

#include "qn_basicTypes.h"
#include "qn_errDef.h"
#include "qn_errors.h"


class BaseNode {
  public:
 
    virtual ~BaseNode(){};

    virtual QN_RESULT apply() = 0;

    virtual QN_RESULT setInput(const std::string &data) {
        std::string inputName;
        Json::Reader reader;
        Json::Value root;
        if (reader.parse(data, root, true))
            if (!root.empty())
                if (root.isObject())
                    if (root.isMember("inputs"))
                        if (root["inputs"].isArray()) {
                            Json::Value value = root["inputs"];
                            Json::ArrayIndex count = value.size();
                            for (Json::ArrayIndex i = 0; i < count; i++) {
                                Json::Value arrayElement = value[i];
                                if (arrayElement.isObject()) {
                                    if (!arrayElement["name"].isString()) {
                                        continue;
                                    }
                                    inputName = arrayElement["name"].asString();
                                    //if name is exist, replace value
									auto itr = m_inputs.find(inputName);
                                    if ( itr != m_inputs.end()) {
                                        m_inputs.at(inputName) = arrayElement.toStyledString();
                                        continue;
                                    }
                                    m_inputs.emplace(inputName, arrayElement.toStyledString());
                                }
                            }
                        }

        return QN_OK;
    }

    virtual QN_RESULT setProperty(const std::string &data) {
        std::string propertyName;
        Json::Reader reader;
        Json::Value root;
        if (reader.parse(data, root, true))
            if (!root.empty())
                if (root.isObject())
                    if (root.isMember("properties"))
                        if (root["properties"].isArray()) {
                            Json::Value value = root["properties"];
                            Json::ArrayIndex count = value.size();
                            for (Json::ArrayIndex i = 0; i < count; i++) {
                                Json::Value arrayElement = value[i];
                                if (arrayElement.isObject()) {
                                    if (!arrayElement["name"].isString()) {
                                        continue;
                                    }
                                    propertyName = arrayElement["name"].asString();
                                    //if name is exist, replace value
									auto itr = m_properties.find(propertyName);
                                    if (itr != m_properties.end()) {
                                        m_properties.at(propertyName) = arrayElement.toStyledString();
                                        continue;
                                    }
                                    m_properties.emplace(propertyName, arrayElement.toStyledString());
                                }
                            }
                        }

        return QN_OK;
    }

    virtual std::string property(const std::string &name, char **value, size_t *len) {
		auto itr = m_properties.find(name);
		if (itr != m_properties.end()) {
            return itr->second;
        }
        return std::string();
    }

    virtual std::string output(const std::string &name) {
		auto itr = m_outputs.find(name);
		if (itr != m_outputs.end()) {
            return itr->second;
        }
        return std::string();
    }
   
    virtual std::string nodeDesc() {
        return m_descriptor;
    }

    virtual std::string nodeName() {
        return m_nodeName;
    }

  protected:

    QN_RESULT hasInput(const std::string &name) const {
		auto itr = m_inputs.find(name);
		if (itr != m_inputs.end()) {
            return QN_OK;
        }
        return QN_NOT_FIND;
    }

	QN_RESULT  hasInputs(std::vector<std::string>& inputs) const {
		QnRtnCode rc;
		for (auto item : inputs) {
			rc = this->hasInput(item);
			if (rc.isFailed()) {
				return rc;
			}
		}
		return QN_OK;
	}

    std::string input(const std::string &name) const {
		auto itr = m_inputs.find(name);
		if (itr != m_inputs.end()) 
            return itr->second;
        return std::string();
    }

    QN_RESULT setOutput(const std::string &name, const std::string &data) {
		auto itr = m_outputs.find(name);
		if ( itr != m_outputs.end()) {
            m_outputs.at(name) = data;
            return QN_OK;
        }
        m_outputs.emplace(name, data);
        return QN_OK;
    }

	QN_RESULT hasProperty(const std::string &name) const {
		auto itr = m_properties.find(name);
		if (itr != m_properties.end()) {
			return QN_OK;
		}
		return QN_NOT_FIND;
	}

	QN_RESULT  hasProperties(std::vector<std::string>& properties) const {
		QnRtnCode rc;
		for (auto property : properties) {
			rc = this->hasProperty(property);
			if (rc.isFailed()) {
				return rc;
			}
		}
		return QN_OK;
	}

	std::string property(const std::string &name) const {
		auto itr = m_properties.find(name);
		if (itr != m_properties.end())
			return itr->second;
		return std::string();
	}

	template <typename T>
	T getValue(std::string data) {
		return T();
	}

	template <>
	bool getValue<bool>(std::string data) {
		Json::Reader reader;
		Json::Value root;

		if (reader.parse(data, root, true))
			if (!root.empty())
				if (root.isObject())
					if (root.isMember("value"))
						if (root["value"].isBool())
							return root["value"].asBool();

		return false;
	}

	template <>
	int getValue<int>(std::string data) {
		Json::Reader reader;
		Json::Value root;

		if (reader.parse(data, root, true))
			if (!root.empty())
				if (root.isObject())
					if (root.isMember("value"))
						if (root["value"].isInt())
							return root["value"].asInt();

		return 0;
	}

	template <>
	unsigned int getValue<unsigned int>(std::string data) {
		Json::Reader reader;
		Json::Value root;

		if (reader.parse(data, root, true))
			if (!root.empty())
				if (root.isObject())
					if (root.isMember("value"))
						if (root["value"].isUInt())
							return root["value"].asUInt();

		return 0;
	}

	template <>
	std::string getValue<std::string>(std::string data) {
		Json::Reader reader;
		Json::Value root;

		if (reader.parse(data, root, true))
			if (!root.empty())
				if (root.isObject())
					if (root.isMember("value"))
						if (root["value"].isString())
							return root["value"].asString();
						else if (root["value"].isObject())
							return root["value"].toStyledString();


		return "";
	}

	template <>
	float getValue<float>(std::string data) {
		Json::Reader reader;
		Json::Value root;

		if (reader.parse(data, root, true))
			if (!root.empty())
				if (root.isObject())
					if (root.isMember("value"))
						if (root["value"].isDouble())
							return root["value"].asFloat();

		return 0.0f;
	}


    std::map<std::string, std::string> m_inputs;
    std::map<std::string, std::string> m_properties;
    std::map<std::string, std::string> m_outputs;

    std::string m_descriptor;
    std::string m_nodeName;

};



//==============================================================================
#endif // EXEPLUGINEXAMPLE_TEST_1_BASENODE_H
//==============================================================================
