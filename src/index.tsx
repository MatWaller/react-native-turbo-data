import TurboData from '../specs/NativeTurboData';

export interface FilterCriteria {
  [key: string]: any;
}

export interface SortCriteria {
  [key: string]: 'asc' | 'desc';
}

export function filterObject(
  dataObject: readonly { [key: string]: any }[],
  filterCriteria: { [key: string]: any },
  sortCriteria?: { [key: string]: 'asc' | 'desc' }
): readonly { [key: string]: any }[] {
  return TurboData.filterObject(dataObject, filterCriteria, sortCriteria);
}
