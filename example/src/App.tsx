import { Text, View, StyleSheet } from 'react-native';
import { filterObject } from 'react-native-turbo-data';

const data = [
  { id: 1, name: 'John', age: 25 },
  { id: 2, name: 'Jane', age: 30 },
  { id: 3, name: 'Bob', age: 25 },
];

const filteredData = filterObject(data, { age: 25 });

export default function App() {
  return (
    <View style={styles.container}>
      {filteredData.map((item) => (
        <Text key={item.id}>
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
