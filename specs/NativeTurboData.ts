import {TurboModule, TurboModuleRegistry} from 'react-native';

export interface Spec extends TurboModule {
  filterObject(dataObject: Object, filterCriteria: Object, sortCriteria?: { [key: string]: 'asc' | 'desc' }): Array<Object> | null;
}

export default TurboModuleRegistry.getEnforcing<Spec>(
    "NativeTurboData"
);