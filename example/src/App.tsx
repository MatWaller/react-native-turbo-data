import { Text, View, StyleSheet } from 'react-native';
import { filterObject } from 'react-native-turbo-data';

type DataType = {
  id: number;
  name: string;
  age: number;
};

const data: DataType[] = [
  { id: 1, name: 'John', age: 25 },
  { id: 2, name: 'Jane', age: 30 },
  { id: 3, name: 'Bob', age: 25 },
];

const filteredData = filterObject(data, { age: 25 }) as DataType[];

export default function App() {
  return (
    <View style={styles.container}>
      {filteredData.map((item: DataType) => (
        <Text key={item.id.toString()}>
          {item.name} (Age: {item.age})
        </Text>
      ))}
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
});
