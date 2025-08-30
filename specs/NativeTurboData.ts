import type {TurboModule} from 'react-native';
import {TurboModuleRegistry} from 'react-native';

export interface Spec extends TurboModule {
  filterObject(dataObject: { [key: string]: any }[], filterCriteria: { [key: string]: any }, sortCriteria?: { [key: string]: 'asc' | 'desc' }): { [key: string]: any }[];
}

export default TurboModuleRegistry.getEnforcing<Spec>(
    "NativeTurboData"
);