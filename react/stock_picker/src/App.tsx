import React from "react";
import {
  G2,
  Chart,
  Tooltip,
  Interval,
  Legend,
  Line,
  View,
  Axis,
  Coordinate
} from "bizcharts";

const data = [
  { name: 'London', 月份: 'Jan.', 月均降雨量: 1 },
  { name: 'London', 月份: 'Feb.', 月均降雨量: 100 },
  { name: 'London', 月份: 'Mar.', 月均降雨量: 39.3 },
  { name: 'London', 月份: 'Apr.', 月均降雨量: 81.4 },
  { name: 'London', 月份: 'May', 月均降雨量: 47 },
  { name: 'London', 月份: 'Jun.', 月均降雨量: 0 },
  { name: 'London', 月份: 'Jul.', 月均降雨量: 24 },
  { name: 'London', 月份: 'Aug.', 月均降雨量: 35.6 },
  { name: 'Berlin', 月份: 'Jan.', 月均降雨量: 12.4 },
  { name: 'Berlin', 月份: 'Feb.', 月均降雨量: 23.2 },
  { name: 'Berlin', 月份: 'Mar.', 月均降雨量: 34.5 },
  { name: 'Berlin', 月份: 'Apr.', 月均降雨量: 99.7 },
  { name: 'Berlin', 月份: 'May', 月均降雨量: 52.6 },
  { name: 'Berlin', 月份: 'Jun.', 月均降雨量: 35.5 },
  { name: 'Berlin', 月份: 'Jul.', 月均降雨量: 37.4 },
  { name: 'Berlin', 月份: 'Aug.', 月均降雨量: 100 },
];

// 数据源
const data1 = [
  {
    month: "Jan",
    city: "Tokyo",
    temperature: -10
  },
  {
    month: "Jan",
    city: "London",
    temperature: 3.9
  },
  {
    month: "Feb",
    city: "Tokyo",
    temperature: 6.9
  },
  {
    month: "Feb",
    city: "London",
    temperature: 4.2
  },
  {
    month: "Mar",
    city: "Tokyo",
    temperature: 9.5
  },
  {
    month: "Mar",
    city: "London",
    temperature: 5.7
  },
  {
    month: "Apr",
    city: "Tokyo",
    temperature: 14.5
  },
  {
    month: "Apr",
    city: "London",
    temperature: 8.5
  },
  {
    month: "May",
    city: "Tokyo",
    temperature: 18.4
  },
  {
    month: "May",
    city: "London",
    temperature: 11.9
  },
  {
    month: "Jun",
    city: "Tokyo",
    temperature: 21.5
  },
  {
    month: "Jun",
    city: "London",
    temperature: 15.2
  },
  {
    month: "Jul",
    city: "Tokyo",
    temperature: 25.2
  },
  {
    month: "Jul",
    city: "London",
    temperature: 17
  },
  {
    month: "Aug",
    city: "Tokyo",
    temperature: 26.5
  },
  {
    month: "Aug",
    city: "London",
    temperature: 16.6
  },
];

const scale = {
  city: {
    formatter: v => {
        return {
          London: '伦敦()',
          Tokyo: '东京()'
        }[v]
      }
    }
  }
  
  
  function App() {
    return (
      
    <Chart scale={scale} padding={[30, 20, 60, 40]} autoFit height={420} data={data1} interactions={['element-active']}>
      <Line shape="smooth" position="month*temperature" color="city" />
      
      <Tooltip shared showCrosshairs region={null} g2-tooltip-list-item={{display:'flex'}}/>
      <Axis visible={false} />
      <Legend name='name' padding={[0,0,5,0]} />
  
      <Legend name='city' padding={[0,0,-8,0]} />
    
      <View height={320} padding={[0, 0, 0, 0]} data={data} autoFit>
        <Interval
          adjust={[
          {
              type: 'dodge',
              marginRatio: 0,
            },
          ]}
          color="name"
          position="月份*月均降雨量"
        />
        <Axis visible={false} />
        <Coordinate scale={[1, 0.2]}/>
      </View>
    </Chart>
  
    );
  }
  
  export default App;
