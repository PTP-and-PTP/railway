import csv
from datetime import datetime

def count_ones_by_day(file_path):
    date_count = {}
    with open(file_path, 'r', newline='') as file:
        reader = csv.reader(file)
        next(reader)  # 跳过CSV文件的表头行
        for row in reader:
            if len(row) >= 2:
                status = int(row[0])
                timestamp_str = row[1]

                if status == 1:
                    timestamp = datetime.strptime(timestamp_str, '%m/%d/%y-%H:%M:%S')
                    date_str = timestamp.strftime('%Y-%m-%d')

                    if date_str in date_count:
                        date_count[date_str] += 1
                    else:
                        date_count[date_str] = 1

    return date_count

if __name__ == '__main__':
    csv_file_path = 'data.csv'
    result = count_ones_by_day(csv_file_path)

    # 将结果数据嵌入HTML模板
    table_rows = ""
    for date, count in result.items():
        table_rows += f"<tr><td>{date}</td><td>{count}</td></tr>"

    with open('./web/pages/statistics/template.html', 'r') as file:
        template_str = file.read()
        html_content = template_str.replace("{{ table_rows }}", table_rows)

    # 将生成的HTML写入文件
    with open('./web/pages/statistics/index.html', 'w') as file:
        file.write(html_content)

