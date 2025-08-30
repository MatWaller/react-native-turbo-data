#include "NativeTurboData.h"
#include <jsi/jsi.h>
#include <algorithm>
#include <vector>
#include <string>

namespace facebook::react {

NativeTurboData::NativeTurboData(std::shared_ptr<CallInvoker> jsInvoker)
    : TurboModule(kModuleName, std::move(jsInvoker)) {}

jsi::Value NativeTurboData::get(jsi::Runtime &rt, const jsi::PropNameID &propName)
    {
        auto name = propName.utf8(rt);

        if (name == "filterObject")
        {
            return jsi::Function::createFromHostFunction(
                rt,
                propName,
                3,
                [this](jsi::Runtime &rt, const jsi::Value &thisValue, const jsi::Value *arguments, size_t count) -> jsi::Value
                {
                    if (count < 2)
                    {
                        throw jsi::JSError(rt, "filterObject expects at least 2 arguments");
                    }

                    if (!arguments[0].isObject() || !arguments[1].isObject())
                    {
                        throw jsi::JSError(rt, "filterObject expects objects as arguments");
                    }

                    jsi::Object dataObject = arguments[0].asObject(rt);
                    jsi::Object filterCriteria = arguments[1].asObject(rt);


                    jsi::Object* sortObj = nullptr;
                    if (count == 3 && arguments[2].isObject()) {
                        sortObj = new jsi::Object(arguments[2].asObject(rt));
                    }

                    jsi::Value result = filterObject(rt, dataObject, filterCriteria, sortObj);
                    
                    if (sortObj != nullptr) {
                        delete sortObj;
                    }

                    return result;
                });
        }

        return TurboModule::get(rt, propName);
    }

    jsi::Array NativeTurboData::filterObject(
        jsi::Runtime &rt,
        const jsi::Object &dataObject,
        const jsi::Object &filterCriteria,
        const jsi::Object *sortCriteria)
    {

        jsi::Array filterKeys = filterCriteria.getPropertyNames(rt);
        size_t filterKeyCount = filterKeys.size(rt);

        jsi::Array dataKeys = dataObject.getPropertyNames(rt);
        size_t dataKeyCount = dataKeys.size(rt);
        if (filterKeyCount == 0)
        {
            return jsi::Array(rt, 0);
        }

        if (filterKeyCount == 1)
        {
            return filterSingleKey(rt, dataObject, filterCriteria);
        }
        std::vector<jsi::Value> matchedItems;
        matchedItems.reserve(dataKeyCount);

        std::vector<std::pair<jsi::PropNameID, jsi::Value>> cachedFilters;
        cachedFilters.reserve(filterKeyCount);
        for (size_t j = 0; j < filterKeyCount; j++)
        {
            jsi::Value filterKeyValue = filterKeys.getValueAtIndex(rt, j);
            if (filterKeyValue.isString())
            {
                std::string filterKey = filterKeyValue.asString(rt).utf8(rt);
                jsi::PropNameID propName = jsi::PropNameID::forUtf8(rt, filterKey);
                jsi::Value expectedValue = filterCriteria.getProperty(rt, propName);
                cachedFilters.emplace_back(std::move(propName), std::move(expectedValue));
            }
        }

        for (size_t i = 0; i < dataKeyCount; i++)
        {
            jsi::Value dataKeyValue = dataKeys.getValueAtIndex(rt, i);
            if (!dataKeyValue.isString())
            {
                continue;
            }

            std::string dataKey = dataKeyValue.asString(rt).utf8(rt);
            jsi::PropNameID dataPropName = jsi::PropNameID::forUtf8(rt, dataKey);
            jsi::Value currentItem = dataObject.getProperty(rt, dataPropName);
            if (!currentItem.isObject())
            {
                continue;
            }

            jsi::Object currentObject = currentItem.asObject(rt);
            bool matchesAllFilters = true;

            for (const auto &[filterPropName, expectedValue] : cachedFilters)
            {
                if (!currentObject.hasProperty(rt, filterPropName))
                {
                    matchesAllFilters = false;
                    break;
                }

                jsi::Value actualValue = currentObject.getProperty(rt, filterPropName);
                bool valuesMatch = false;
                if (expectedValue.isObject() && actualValue.isObject())
                {
                    jsi::Object expectedObj = expectedValue.asObject(rt);
                    jsi::Object actualObj = actualValue.asObject(rt);

                    jsi::Array expectedProps = expectedObj.getPropertyNames(rt);
                    size_t expectedPropCount = expectedProps.size(rt);

                    valuesMatch = true;
                    for (size_t k = 0; k < expectedPropCount; k++)
                    {
                        jsi::Value propKeyValue = expectedProps.getValueAtIndex(rt, k);
                        if (!propKeyValue.isString())
                            continue;

                        std::string propKey = propKeyValue.asString(rt).utf8(rt);
                        jsi::PropNameID propName = jsi::PropNameID::forUtf8(rt, propKey);

                        if (!actualObj.hasProperty(rt, propName))
                        {
                            valuesMatch = false;
                            break;
                        }

                        jsi::Value expectedNestedValue = expectedObj.getProperty(rt, propName);
                        jsi::Value actualNestedValue = actualObj.getProperty(rt, propName);


                        if (expectedNestedValue.isString() && actualNestedValue.isString())
                        {
                            if (expectedNestedValue.asString(rt).utf8(rt) != actualNestedValue.asString(rt).utf8(rt))
                            {
                                valuesMatch = false;
                                break;
                            }
                        }
                        else if (expectedNestedValue.isNumber() && actualNestedValue.isNumber())
                        {
                            if (expectedNestedValue.asNumber() != actualNestedValue.asNumber())
                            {
                                valuesMatch = false;
                                break;
                            }
                        }
                        else if (expectedNestedValue.isBool() && actualNestedValue.isBool())
                        {
                            if (expectedNestedValue.asBool() != actualNestedValue.asBool())
                            {
                                valuesMatch = false;
                                break;
                            }
                        }
                        else
                        {
                            valuesMatch = false;
                            break;
                        }
                    }
                }
                else if (expectedValue.isString() && actualValue.isString())
                {
                    valuesMatch = expectedValue.asString(rt).utf8(rt) == actualValue.asString(rt).utf8(rt);
                }
                else if (expectedValue.isNumber() && actualValue.isNumber())
                {
                    valuesMatch = expectedValue.asNumber() == actualValue.asNumber();
                }
                else if (expectedValue.isBool() && actualValue.isBool())
                {
                    valuesMatch = expectedValue.asBool() == actualValue.asBool();
                }

                if (!valuesMatch)
                {
                    matchesAllFilters = false;
                    break;
                }
            }

            if (matchesAllFilters)
            {
                matchedItems.push_back(std::move(currentItem));
            }
        }

        jsi::Array resultArray(rt, matchedItems.size());
        for (size_t i = 0; i < matchedItems.size(); i++)
        {
            resultArray.setValueAtIndex(rt, i, std::move(matchedItems[i]));
        }


        if (sortCriteria != nullptr)
        {
            jsi::Array sortKeys = sortCriteria->getPropertyNames(rt);
            size_t sortKeyCount = sortKeys.size(rt);

            if (sortKeyCount > 0)
            {
                // Convert to vector for sorting
                std::vector<std::pair<jsi::Object, std::vector<jsi::Value>>> sortableItems;
                sortableItems.reserve(matchedItems.size());

                // Prepare sorting keys
                std::vector<std::string> sortPropertyKeys;
                std::vector<bool> sortDirections;
                sortPropertyKeys.reserve(sortKeyCount);
                sortDirections.reserve(sortKeyCount);

                // Cache sort criteria
                for (size_t i = 0; i < sortKeyCount; i++)
                {
                    jsi::Value keyValue = sortKeys.getValueAtIndex(rt, i);
                    if (!keyValue.isString())
                        continue;

                    std::string key = keyValue.asString(rt).utf8(rt);
                    jsi::Value directionValue = sortCriteria->getProperty(rt, jsi::PropNameID::forUtf8(rt, key));

                    sortPropertyKeys.push_back(key);
                    sortDirections.push_back(directionValue.isString() &&
                                                     directionValue.asString(rt).utf8(rt) == "desc"
                                                 ? false
                                                 : true);
                }

                // Prepare items for sorting
                for (auto &item : matchedItems)
                {
                    if (!item.isObject())
                        continue;

                    jsi::Object obj = item.asObject(rt);
                    std::vector<jsi::Value> sortValues;
                    sortValues.reserve(sortKeyCount);

                    for (const auto &key : sortPropertyKeys)
                    {
                        jsi::PropNameID propName = jsi::PropNameID::forUtf8(rt, key);
                        if (obj.hasProperty(rt, propName))
                        {
                            sortValues.push_back(obj.getProperty(rt, propName));
                        }
                        else
                        {
                            sortValues.push_back(jsi::Value::null());
                        }
                    }

                    sortableItems.emplace_back(std::move(obj), std::move(sortValues));
                }

                // Sort items
                std::sort(sortableItems.begin(), sortableItems.end(),
                          [&rt, &sortDirections](const auto &a, const auto &b)
                          {
                              for (size_t i = 0; i < a.second.size(); i++)
                              {
                                  const auto &aVal = a.second[i];
                                  const auto &bVal = b.second[i];
                                  bool ascending = sortDirections[i];

                                  if (aVal.isString() && bVal.isString())
                                  {
                                      int cmp = aVal.asString(rt).utf8(rt).compare(bVal.asString(rt).utf8(rt));
                                      if (cmp != 0)
                                          return ascending ? cmp < 0 : cmp > 0;
                                  }
                                  else if (aVal.isNumber() && bVal.isNumber())
                                  {
                                      double diff = aVal.asNumber() - bVal.asNumber();
                                      if (diff != 0)
                                          return ascending ? diff < 0 : diff > 0;
                                  }
                              }
                              return false;
                          });

                // Create sorted result array
                jsi::Array sortedArray(rt, sortableItems.size());
                for (size_t i = 0; i < sortableItems.size(); i++)
                {
                    sortedArray.setValueAtIndex(rt, i, std::move(sortableItems[i].first));
                }

                return sortedArray;
            }
        }
        return resultArray;
    }

    jsi::Array NativeTurboData::filterSingleKey(
        jsi::Runtime& rt,
        const jsi::Object& dataObject,
        const jsi::Object& filterCriteria,
        const jsi::Object* sortCriteria) {

        jsi::Array filterKeys = filterCriteria.getPropertyNames(rt);
        jsi::Value filterKeyValue = filterKeys.getValueAtIndex(rt, 0);
        std::string filterKey = filterKeyValue.asString(rt).utf8(rt);
        jsi::PropNameID filterPropName = jsi::PropNameID::forUtf8(rt, filterKey);
        jsi::Value expectedValue = filterCriteria.getProperty(rt, filterPropName);

        jsi::Array dataKeys = dataObject.getPropertyNames(rt);
        size_t dataKeyCount = dataKeys.size(rt);

        std::vector<jsi::Value> matchedItems;
        matchedItems.reserve(dataKeyCount);

        for (size_t i = 0; i < dataKeyCount; i++) {
            jsi::Value dataKeyValue = dataKeys.getValueAtIndex(rt, i);
            if (!dataKeyValue.isString()) continue;

            jsi::PropNameID dataPropName = jsi::PropNameID::forUtf8(rt, dataKeyValue.asString(rt).utf8(rt));
            jsi::Value currentItem = dataObject.getProperty(rt, dataPropName);

            if (!currentItem.isObject()) continue;

            jsi::Object currentObject = currentItem.asObject(rt);
            
            if (!currentObject.hasProperty(rt, filterPropName)) continue;

            jsi::Value actualValue = currentObject.getProperty(rt, filterPropName);
            bool matches = false;

            if (expectedValue.isString() && actualValue.isString()) {
                matches = expectedValue.asString(rt).utf8(rt) == actualValue.asString(rt).utf8(rt);
            } else if (expectedValue.isNumber() && actualValue.isNumber()) {
                matches = expectedValue.asNumber() == actualValue.asNumber();
            } else if (expectedValue.isBool() && actualValue.isBool()) {
                matches = expectedValue.asBool() == actualValue.asBool();
            }

            if (matches) {
                matchedItems.push_back(std::move(currentItem));
            }
        }

        jsi::Array resultArray(rt, matchedItems.size());
        for (size_t i = 0; i < matchedItems.size(); i++) {
            resultArray.setValueAtIndex(rt, i, std::move(matchedItems[i]));
        }


        if (sortCriteria != nullptr) {
            jsi::Array sortKeys = sortCriteria->getPropertyNames(rt);
            size_t sortKeyCount = sortKeys.size(rt);

            if (sortKeyCount > 0) {
                // Convert to vector for sorting
                std::vector<std::pair<jsi::Object, std::vector<jsi::Value>>> sortableItems;
                sortableItems.reserve(matchedItems.size());

                // Prepare sorting keys
                std::vector<std::string> sortPropertyKeys;
                std::vector<bool> sortDirections;
                sortPropertyKeys.reserve(sortKeyCount);
                sortDirections.reserve(sortKeyCount);

                // Cache sort criteria
                for (size_t i = 0; i < sortKeyCount; i++) {
                    jsi::Value keyValue = sortKeys.getValueAtIndex(rt, i);
                    if (!keyValue.isString()) continue;

                    std::string key = keyValue.asString(rt).utf8(rt);
                    jsi::Value directionValue = sortCriteria->getProperty(rt, jsi::PropNameID::forUtf8(rt, key));
                    
                    sortPropertyKeys.push_back(key);
                    sortDirections.push_back(directionValue.isString() && 
                        directionValue.asString(rt).utf8(rt) == "desc" ? false : true);
                }

                // Prepare items for sorting
                for (auto& item : matchedItems) {
                    if (!item.isObject()) continue;
                    
                    jsi::Object obj = item.asObject(rt);
                    std::vector<jsi::Value> sortValues;
                    sortValues.reserve(sortKeyCount);

                    for (const auto& key : sortPropertyKeys) {
                        jsi::PropNameID propName = jsi::PropNameID::forUtf8(rt, key);
                        if (obj.hasProperty(rt, propName)) {
                            sortValues.push_back(obj.getProperty(rt, propName));
                        } else {
                            sortValues.push_back(jsi::Value::null());
                        }
                    }

                    sortableItems.emplace_back(std::move(obj), std::move(sortValues));
                }

                // Sort items
                std::sort(sortableItems.begin(), sortableItems.end(), 
                    [&rt, &sortDirections](const auto& a, const auto& b) {
                        for (size_t i = 0; i < a.second.size(); i++) {
                            const auto& aVal = a.second[i];
                            const auto& bVal = b.second[i];
                            bool ascending = sortDirections[i];

                            if (aVal.isString() && bVal.isString()) {
                                int cmp = aVal.asString(rt).utf8(rt).compare(bVal.asString(rt).utf8(rt));
                                if (cmp != 0) return ascending ? cmp < 0 : cmp > 0;
                            }
                            else if (aVal.isNumber() && bVal.isNumber()) {
                                double diff = aVal.asNumber() - bVal.asNumber();
                                if (diff != 0) return ascending ? diff < 0 : diff > 0;
                            }
                        }
                        return false;
                    });

                // Create sorted result array
                jsi::Array sortedArray(rt, sortableItems.size());
                for (size_t i = 0; i < sortableItems.size(); i++) {
                    sortedArray.setValueAtIndex(rt, i, std::move(sortableItems[i].first));
                }
                
                return sortedArray;
            }
        }

        return resultArray;
    }
}