import TurboData from './NativeTurboData';

export function filterObject(
  dataObject: readonly {
    [key: string]: string | number | boolean | null | object;
  }[],
  filterCriteria: {
    [key: string]: string | number | boolean | null | object;
  },
  sortCriteria?: { [key: string]: 'asc' | 'desc' }
): readonly {
  [key: string]: string | number | boolean | null | object;
}[] {
  return TurboData.filterObject(dataObject, filterCriteria, sortCriteria);
}
