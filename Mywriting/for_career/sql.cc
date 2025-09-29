/*

mysql学习：https://frxcat.fun/database/MySQL/MySQL-CRUD/


SELECT [DISTINCT] 列名/表达式  -- 5. 确定最终返回的列
FROM 表名                      -- 1. 指定数据来源表
[JOIN 表名 ON 连接条件]        -- 1. （可选）关联其他表
WHERE 条件                     -- 2. 过滤行（分组前过滤）
GROUP BY 列名                  -- 3. 按列分组
HAVING 条件                    -- 4. 过滤分组（分组后过滤）
ORDER BY 列名 [ASC/DESC]       -- 6. 对结果排序
LIMIT 数量                     -- 7. （可选）限制返回行数

创建数据表
creat [temporeary] table [if not exist] 表名

查看数据表
show tables [like 匹配模式],若无匹配模式则查询所有表
LIKE 后的匹配模式必须使用单引号或双引号包裹

修改数据表名
alert table 旧表名 rename [to | as] 新表名

修改数据表选项
alert table 旧表名 选项=值

查看建表语句
show create table 表名

修改表字段名
ALTER TABLE 数据表名 CHANGE [COLUMN] 旧字段名 新字段名 字段类型 [字段属性];

删除数据表
drop [temprorary] table [if exist] 表名

查询数据
select {* | 字段名} from 数据表名 where 字段名=值

修改数据
update table set 字段名=值 wherer 条件表达式

删除数据
delete from table where 条件表达式

*/