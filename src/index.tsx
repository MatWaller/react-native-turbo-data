import TurboData from './NativeTurboData';

export interface FilterCriteria {
  [key: string]: any;
}

export interface SortCriteria {
  [key: string]: 'asc' | 'desc';
}

export function filterObject(
  dataObject: Object,
  filterCriteria: FilterCriteria,
  sortCriteria?: SortCriteria
): Array<Object> | null {
  return TurboData.filterObject(dataObject, filterCriteria, sortCriteria);
}
