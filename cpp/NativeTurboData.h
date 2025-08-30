#pragma once

#include <ReactCommon/TurboModule.h>
#include <jsi/jsi.h>

#include <memory>
#include <string>

namespace facebook::react {
    class NativeTurboData : public TurboModule {
    public:
        static constexpr auto kModuleName = "NativeTurboData";

        NativeTurboData(std::shared_ptr<CallInvoker> jsInvoker);        // Override the get method to expose functions to JavaScript
        jsi::Value get(jsi::Runtime& rt, const jsi::PropNameID& propName) override;

        jsi::Array filterObject(
            jsi::Runtime& rt,
            const jsi::Object& dataObject,
            const jsi::Object& filterCriteria,
            const jsi::Object* sortCriteria = nullptr);

    private:
        jsi::Array filterSingleKey(
            jsi::Runtime& rt,
            const jsi::Object& dataObject,
            const jsi::Object& filterCriteria,
            const jsi::Object* sortCriteria = nullptr
        );
    };
}