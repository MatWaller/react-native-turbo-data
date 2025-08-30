package com.turbodata

import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.module.annotations.ReactModule

@ReactModule(name = TurboDataModule.NAME)
class TurboDataModule(reactContext: ReactApplicationContext) :
  NativeTurboDataSpec(reactContext) {

  override fun getName(): String {
    return NAME
  }

  override fun filterObject(dataObject: ReadableArray, filterCriteria: ReadableMap, sortCriteria: ReadableMap?): ReadableArray {
    // The actual implementation is in the C++ files
    // This Kotlin class just serves as the registration point for the TurboModule
    throw UnsupportedOperationException("Not implemented: The actual implementation is in C++")
  }

  companion object {
    const val NAME = "TurboData"
  }
}
