import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

export interface Spec extends TurboModule {
  filterObject(
    dataObject: readonly {
      [key: string]: string | number | boolean | null | object;
    }[],
    filterCriteria: {
      [key: string]: string | number | boolean | null | object;
    },
    sortCriteria?: { [key: string]: 'asc' | 'desc' }
  ): readonly {
    [key: string]: string | number | boolean | null | object;
  }[];
}

export default TurboModuleRegistry.getEnforcing<Spec>('NativeTurboData');
