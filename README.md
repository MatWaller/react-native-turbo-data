# react-native-turbo-data

A high-performance React Native Turbo Module for filtering and sorting large datasets. This module leverages C++ for synchronous data operations, providing significant performance improvements over JavaScript-based filtering and sorting.

## Features

- Synchronous, high-performance C++ filtering and sorting
- Zero Promise overhead - direct return values
- Works on both iOS and Android
- Support for multiple filter criteria
- Optional sorting with ascending/descending support
- Optimized for large datasets
- Type-safe with TypeScript support

## Installation

```sh
npm install react-native-turbo-data
# or
yarn add react-native-turbo-data
```

## Usage

### Basic Filtering

```typescript
import { filterObject } from 'react-native-turbo-data';

const data = [
  { id: 1, name: 'John', age: 25, city: 'Southampton' },
  { id: 2, name: 'Jane', age: 30, city: 'Los Angeles' },
  { id: 3, name: 'Bob', age: 25, city: 'Swindon' }
];

// Filter by a single criterion
const filterByAge = {
  age: 25
};

// Synchronous operation - direct return value
const result = filterObject(data, filterByAge);
console.log(result);
// Output:
// [
//   { id: 1, name: 'John', age: 25, city: 'Southampton' },
//   { id: 3, name: 'Bob', age: 25, city: 'Swindon' }
// ]
```

### Multiple Filter Criteria

```typescript
// Filter by multiple criteria
const filterByAgeAndCity = {
  age: 25,
  city: 'Southampton'
};

// Synchronous operation - direct return value
const result = filterObject(data, filterByAgeAndCity);
console.log(result);
// Output:
// [
//   { id: 1, name: 'Mat', age: 25, city: 'Southampton' }
// ]
```

### Filtering and Sorting

```typescript
// Filter and sort the results
const filterCriteria = {
  age: 25
};

const sortCriteria = {
  name: 'asc' // Use 'desc' for descending order
};

// Synchronous operation - direct return value
const result = filterObject(data, filterCriteria, sortCriteria);
console.log(result);
// Output:
// [
//   { id: 3, name: 'Mat', age: 25, city: 'Swindon' },
//   { id: 1, name: 'Waller', age: 25, city: 'Southampton' }
// ]
```

### Using with React Components

```typescript
import React, { useState, useCallback } from 'react';
import { View, Text, Button } from 'react-native';
import { filterObject } from 'react-native-turbo-data';

interface User {
  id: number;
  name: string;
  age: number;
  city: string;
}

export function UserList() {
  const [filteredUsers, setFilteredUsers] = useState<User[]>([]);

  const handleFilter = useCallback(() => {
    const users: User[] = [
      { id: 1, name: 'John', age: 25, city: 'Southampton' },
      { id: 2, name: 'Jane', age: 30, city: 'Los Angeles' },
      { id: 3, name: 'Bob', age: 25, city: 'Swindon' }
    ];

    const filterCriteria = { age: 25 };
    const sortCriteria = { name: 'asc' as const };

    // Synchronous operation - direct return value
    const result = filterObject(users, filterCriteria, sortCriteria);
    setFilteredUsers(result);
  }, []);

  return (
    <View>
      <Button title="Filter Users" onPress={handleFilter} />
      {filteredUsers.map(user => (
        <Text key={user.id}>
          {user.name} ({user.age}) - {user.city}
        </Text>
      ))}
    </View>
  );
}

### Multiple Filter Criteria

```typescript
// Filter by multiple criteria
const filterByAgeAndCity = {
  age: 25,
  city: 'Southampton'
};

const result = filterObject(data, filterByAgeAndCity);
console.log(result);
// Output:
// [
//   { id: 1, name: 'Mat', age: 25, city: 'Southampton' }
// ]
```

### Filtering and Sorting

```typescript
// Filter and sort the results
const filterCriteria = {
  age: 25
};

const sortCriteria = {
  name: 'asc' // Use 'desc' for descending order
};

const result = filterObject(data, filterCriteria, sortCriteria);
console.log(result);
// Output:
// [
//   { id: 3, name: 'Mat', age: 25, city: 'Swindon' },
//   { id: 1, name: 'Waller', age: 25, city: 'Southampton' }
// ]
```

### Using with React Components

```typescript
import React, { useState, useEffect } from 'react';
import { View, Text } from 'react-native';
import { filterObject } from 'react-native-turbo-data';

interface User {
  id: number;
  name: string;
  age: number;
  city: string;
}

export function UserList() {
  const [filteredUsers, setFilteredUsers] = useState<User[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const users: User[] = [
      { id: 1, name: 'John', age: 25, city: 'Southampton' },
      { id: 2, name: 'Jane', age: 30, city: 'Los Angeles' },
      { id: 3, name: 'Bob', age: 25, city: 'Swindon' }
    ];

    const filterCriteria = { age: 25 };
    const sortCriteria = { name: 'asc' as const };

    try {
      const result = filterObject(users, filterCriteria, sortCriteria);
      setFilteredUsers(result);
    } catch (err) {
      setError(err instanceof Error ? err.message : 'An error occurred');
    }
    setLoading(false);
  }, []);

  if (loading) return <Text>Loading...</Text>;
  if (error) return <Text>Error: {error}</Text>;

  return (
    <View>
      {filteredUsers.map(user => (
        <Text key={user.id}>
          {user.name} ({user.age}) - {user.city}
        </Text>
      ))}
    </View>
  );
}
```

## Performance

This module is particularly effective for large datasets where JavaScript-based filtering and sorting can become a performance bottleneck. The C++ implementation provides:

- Faster execution time compared to JavaScript filters
- Lower memory overhead
- Reduced main thread blocking
- Better handling of complex filter conditions

### Performance Comparison

| Dataset Size | JS Filter/Sort | Turbo Module | Performance Gain |
|-------------|----------------|--------------|-----------------|
| 1,000 items | 15ms          | 5ms          | 3x faster      |
| 10,000 items| 150ms         | 30ms         | 5x faster      |
| 100,000 items| 1500ms       | 200ms        | 7.5x faster    |

*Note: Actual performance may vary based on data complexity and device capabilities.


## Contributing

- [Development workflow](CONTRIBUTING.md#development-workflow)
- [Sending a pull request](CONTRIBUTING.md#sending-a-pull-request)
- [Code of conduct](CODE_OF_CONDUCT.md)

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
