import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

export interface FilterableRecord {
  value: {
    stringValue?: string;
    numberValue?: number;
    booleanValue?: boolean;
  };
  field: string;
}

export interface SortConfig {
  field: string;
  direction: string;
}

export interface Spec extends TurboModule {
  filterObject(
    dataObject: Array<FilterableRecord>,
    filterCriteria: FilterableRecord,
    sortCriteria?: SortConfig
  ): Array<FilterableRecord>;
}

export default TurboModuleRegistry.getEnforcing<Spec>('NativeTurboData');
