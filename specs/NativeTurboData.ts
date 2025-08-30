import type {TurboModule} from 'react-native';
import {TurboModuleRegistry} from 'react-native';

export interface Spec extends TurboModule {
  filterObject(
    dataObject: readonly { [key: string]: any }[],
    filterCriteria: { [key: string]: any },
    sortCriteria?: { [key: string]: 'asc' | 'desc' }
  ): readonly { [key: string]: any }[];
}

export default TurboModuleRegistry.getEnforcing<Spec>(
    "NativeTurboData"
);