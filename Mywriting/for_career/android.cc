/*
安卓开发八股：
第一行代码：https://juejin.cn/post/7257865786265600057

C++和java的区别：
内存管理上:c++允许程序员手动开辟内存以及内存的释放，但java是通过gc垃圾回收器进行内存回收的
跨平台性上：c++需要为不同的操作系统生成可执行文件，但是java是由jvm在不同的平台上进行解释或编译执行
异常处理：c++很多未定义的行为会导致程序直接崩溃，java很多错误被包装成了可以捕捉的异常

编译型语言和解释型语言：
编译型语言是将代码一次性编译为二进制文件，然后再执行。例如c / c++
解释型语言是一行一行的读取源代码，然后将源代码实时编译为二进制代码，然后再执行，效率比较低。例如
js和python。
java和kotlin都是属于解释型语言，因为都是先将源代码变成class文件，然后再由jvm解释为二进制语言，所以属于
解释型语言。kotlin相比于java的语言更简单和高级.


安卓四大核心组件：Activity,Service,BroadcastReceiver,ContentProvider
intent的使用：1.显示intent，直接定义出intent,并向intent中添加需要的数据，然后放入
startActivity(intent)进入下一个activity。2.隐式intent，在配置文件中去添加相应的action和category

activity的生命周期：onCreat,onStart,onResume,OnPause,OnStop,OnDestroy,OnRestart
activity的启动模式:1.standard 2.singletop 3.singletask 4.singleInstance
启动activity的最佳写法：通过Intent意图来指定启动的activity，并且可以传入需要传递的内容

怎么保证重新启动的activity是在另外一个栈中：
1.在配置文件中配置activity的启动模式为singleInstance
<activity
    android:name=".YourActivity"
    android:launchMode="singleInstance">
</activity>
2.启动activity时，通过intent添加FLAG_ACTIVITY_NEW_TASK 标志
Intent intent = new Intent(context, YourActivity.class);
// 添加这个标志会使Activity在新的任务栈中启动
intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
context.startActivity(intent);


companion伴生对象：kotlin中没有java和c++中的static定义的静态变量，companion object可以起到相似的作用
静态方法又称为类方法，不需要对象实例也能进行调用，kotlin中通过object实现的单例类，在单例类中的所有
方法都类似于静态方法的调用。如果只想类中的某一个方法成为静态方法，可通过companion object来实现

kotlin中的with函数，提供两个参数，第一个是任意类型的对象，第二个是一个lambda表达式,在lambda表达式中
提供了第一个参数的上下文，并将lambda表达式中最后一行代码作为返回值返回
示例：
fun eatfruits_1() {
    val list = listOf("Apple", "Banana", "Orange", "Pear", "Grape")
        //with第一个参数传入StringBuilder对象，接下来Lambda的上下文都是该StringBuilder对象。
        val result = with(StringBuilder()) {
        append("Starting eat fruit\n")
            for (fruit in list) {
                append(fruit).append("\n")
            }
        append("Ate all fruits.\n")
            //Lambda的最后一行作为with函数的返回值返回，最终将结果打印出来
            toString()
    }
    println(result)
}

kotlin中的run函数和with函数比较类似，不过run函数需要通过具体对象进行调用，只接受一个lambda表达式，
在lambda表达式中提供调用对象的上下文,并将表达式最后一行作为返回值返回
示例：
fun eatfruits_2() {
    val list = listOf("Apple", "Banana", "Orange", "Pear", "Grape")
        val result = StringBuilder().run{
            append("Starting eat fruit\n")
            for (fruit in list) {
                append(fruit).append("\n")
            }
            append("Ate all fruits.\n")
            toString()
    }
        println(result)
}

kotlin中的apply函数和run函数及其类似，也是需要通过具体的对象进行调用，接收一个lamda表达式，表达式中
提供了调用对象的上下文，但是不同的是返回的是调用对象本身


控件：
TextView:gravity表示文字的对齐方式
EditText:maxlines指明最大行数，hint表示提示内容
ImageView:src表示图片路径

ListView：1.准备数据源 2.获取listview实例 3.创建适配器 4.将适配器绑定到listview 5.设置点击事件
ListView快速滑动时，性能成为一个瓶颈，在这里我们借助了两种方案：getView的convertView参数
和ViewHolder进行性能优化。convertView用于将之前已经加载好的布局进行缓存，以便以后重用；
如果convertView为空，使用LayoutInflater直接加载，如果不为空，直接对convertView进行重用。
优化convertView后，虽然已经不会加载重复布局，但仍然在调用getView方法时调用view的findviewByID
获取控件实例，因此借助ViewHolder来进行优化。


if (convertView == null) {
    view = LayoutInflater.from(context).inflate(resourceId, parent, false)
        //用于对ImageView和TextView的控件实例进行缓存
        val fruitImage : ImageView = view.findViewById(R.id.fruit_image)
        val fruitName : TextView = view.findViewById(R.id.fruit_Name)
        //创建ViewHolder对象并将控件实例放在ViewHolder里
        viewHolder = ViewHolder(fruitImage, fruitName)
        //使用View的setTag方法将ViewHolder存储在View中。
        view.tag = viewHolder
}
else {
    view = convertView
        //缓存对象不为空时，使用View的getTag方法将ViewHolder重新取出，这样所有控件的实例都存储于ViewHolder中了
        viewHolder = view.tag as ViewHolder
}

//得到当前项的Fruit实例
val fruit = getItem(position)
//设置图片和文字
if (fruit != null) {
    viewHolder.fruitImage.setImageResource(fruit.imageid)
        viewHolder.fruitName.text = fruit.name
}


recycleview共有四级缓存：
1. 一级缓存：mAttachedScrap（附加的废弃视图）
作用：缓存当前屏幕上可见的 ViewHolder。
特点：
存储当前仍 “附着” 在 RecyclerView 上的 ViewHolder（即处于屏幕内或刚滑出屏幕边缘的 Item）
使用场景：
当 RecyclerView 触发重新布局（如调用 notifyDataSetChanged、屏幕旋转）时，会先将当前所有可见的 ViewHolder 存入 mAttachedScrap。
布局完成后，若这些 ViewHolder 仍在可见范围内，直接从缓存中取出复用，无需重新创建。
2. 二级缓存：mCachedViews（缓存视图）
作用：缓存最近从屏幕上移除的 ViewHolder，且保留其绑定的数据。
特点：
容量默认是2个（可通过 setItemViewCacheSize(int) 调整）。
存储的 ViewHolder 是「完全绑定了数据」的，复用它们时无需调用 onBindViewHolder()，直接显示即可。
使用场景：
适用于「往返滑动」场景（如用户滑动列表后又滑回来），能快速恢复之前的 Item 状态，避免重复绑定数据。
当缓存数量超过上限时，会按「LRU（最近最少使用）」规则将最久未使用的 ViewHolder 移至四级缓存。
3. 三级缓存：mViewCacheExtension（自定义缓存扩展）
作用：开发者自定义的缓存，用于特殊场景下的视图复用。
特点：
RecyclerView 本身不实现此缓存，需开发者通过 setViewCacheExtension(ViewCacheExtension) 自定义逻辑。
优先级介于 mCachedViews 和 RecyclerPool 之间。
使用场景：
适用于有特殊复用需求的场景，例如：固定位置的 Item（如列表头部 / 尾部）需要优先复用。
4. 四级缓存：RecyclerPool（回收池）
作用：缓存不同类型的 ViewHolder，但会清除其绑定的数据。
特点：
按「ViewType」分类存储，每种类型默认最多缓存 5 个 ViewHolder（可通过 setMaxRecycledViews(int viewType, int max) 调整）。
存储的 ViewHolder 是「未绑定数据」的，复用前必须调用 onBindViewHolder() 重新绑定数据。
使用场景：
当 mCachedViews 满了之后，多余的 ViewHolder 会被移到这里。
跨 RecyclerView 共享：多个 RecyclerView 可以共享同一个 RecyclerPool（通过 setRecycledViewPool(RecyclerView.RecycledViewPool)），进一步提升复用效率（如 ViewPager 中的多个列表）。
缓存的读取顺序
当 RecyclerView 需要获取 ViewHolder 时，会按以下顺序查找缓存：
mAttachedScrap → 2. mCachedViews → 3. mViewCacheExtension → 4. RecyclerPool
如果四级缓存都没有可用的 ViewHolder，则通过 Adapter.onCreateViewHolder() 新建。

RecycleVeiw是怎样实现视图复用的：
RecyclerView 的复用机制基于「回收池」（RecyclerPool和「视图持有者」（ViewHolder），
核心逻辑是：只创建屏幕可见范围内的视图，当视图滚动出屏幕时，将其回收至池中，待新数据需要
展示时重新取出复用，而非为每个数据项都创建新视图。
1.初始化阶段：创建初始可见视图
当 RecyclerView 首次加载时，会根据屏幕可显示的条目数量，通过 Adapter 创建对应数量的
ViewHolder（每个 ViewHolder 持有一个 Item 视图），这些视图会填充初始数据并显示在屏幕上。
例如：屏幕一次能显示 8 个条目，就只创建 8 个 ViewHolder。
2.滚动时：回收与复用视图
当用户向上滚动列表时：出屏视图被回收：顶部的条目滚动出屏幕后，RecyclerView 会将其对应的 ViewHolder
标记为「可复用」，并放入 「回收池」（RecyclerPool） 中，此时视图的引用被保留，但数据会被清除。
入屏视图复用回收池中的视图：当新的条目需要从底部进入屏幕时，RecyclerView 会先检查回收池是否有可用的
ViewHolder：
若有，则直接复用该 ViewHolder，通过 Adapter 的 onBindViewHolder() 方法为其绑定新数据，无需重新创建视图。
若没有（如列表刚加载时），才会通过 Adapter 的 onCreateViewHolder() 方法创建新的 ViewHolder。
3.回收池（RecyclerPool）的细节
回收池按 「视图类型」（ViewType） 分类存储 ViewHolder，不同类型的视图（如列表中混合的文字项、图片项）不会互相复用。
回收池有默认容量限制（每种类型默认最多缓存 5 个 ViewHolder），超出容量的会被销毁，避免占用过多内存。

————————————————

RecyclerView:与listview相比的优势就是，Listview只能纵向滑动，但是RecyclerView可以实现横向滑动，
这是因为listview是基于自身管理的，而recyclerView是基于布局进行管理的，RecyclerView没有
setOnItemClickListener这样的注册监听器方法，而是需要我们给子项具体的View去注册点击事件以下是比
listview多出来的部分.

RecycleView中能够嵌套listView吗？
不推荐这样做，因为
1.滑动冲突：RecyclerView和ListView 都是可滑动控件，嵌套时会导致滑动事件争夺（例如内外层滑动方向一致时，
系统难以判断用户想滑动哪一层），出现滑动不流畅、卡顿甚至无法滑动的情况
2.过度绘制与内存消耗：两者都是适配器模式的列表控件，嵌套会导致多层视图层级，增加绘制压力和内存占用
3.回收复用效率低：RecyclerView的优势在于ViewHolder 的高效回收复用，而嵌套 ListView 会破坏这种机制，
导致大量视图频繁创建和销毁，引发性能下降（尤其数据量大时）。
。

冲突点一：ListView 的复用池失效
当 ListView 作为 RecyclerView 的一个列表项时，灾难就发生了：
场景：想象一下，你的 RecyclerView 有 10 个列表项，每个都是一个 ListView。当你向上滑动 RecyclerView 时，
第一个 ListView（我们称之为 LV1）整体滑出了屏幕。RecyclerView 的操作：RecyclerView 检测到 LV1 滑出屏幕，
于是将包含 LV1 的那个 RecyclerView 的 ViewHolder（我们称之为 RV_VH1）整个回收，并放入 RecyclerView 的全局复用池中。
ListView 的悲剧：在 RV_VH1 被回收的那一刻，它内部的 LV1 及其所有子项都被作为一个整体的 View 树保存了起来。
LV1 自身的复用池（用于复用其内部列表项的）被完全忽略了。
后果：当 RV_VH1 再次被 RecyclerView 复用（比如滑回屏幕）时，它会带着上次被回收时的 LV1 一起被复用。
LV1 需要重新绑定数据并刷新，但由于它自己的复用池在回收期间已经 “死亡”，它无法复用任何之前创建的子项 View。
结果就是，每次 ListView 被复用，它都必须从头开始创建所有的子项 View，完全丧失了复用的意义，性能急剧下降。
一句话总结：ListView 被 RecyclerView 当成一个 “不可分割的整体” 来回收和复用，导致其内部的、更精细的子项复用机制完全失效

冲突点二：RecyclerView 的测量与布局被破坏
RecyclerView 为了高效地滚动，做了很多优化，其中之一就是预测性布局和高效的测量。它会尝试计算出屏幕外的列表项高度，
以便更平滑地滚动。当内部嵌套 ListView 时，这个过程会被彻底打乱：
高度无法预测：ListView 的高度是根据其内容动态计算的。当 RecyclerView 尝试测量包含 ListView 的列表项时，
它必须触发 ListView 的完整测量和布局过程，而 ListView 又必须测量其所有子项才能确定自己的高度。
这是一个递归的、极其昂贵的测量过程。wrap_content 的灾难：如果 ListView 的高度设置为 wrap_content，情况会更糟。
每次 RecyclerView 布局时，都可能导致 ListView 重新测量所有子项。当数据量很大时，这会造成严重的性能瓶颈，
导致界面卡顿甚至 ANR。滚动冲突：虽然不是直接关于复用，但嵌套滚动也会带来用户体验问题。
内外两层列表的滚动事件可能会相互干扰，导致滚动不流畅或不符合预期。


同样listview也不能嵌套recycleview:
1. ListView 的复用机制导致 RecyclerView 频繁重建，丧失复用价值
ListView 的复用逻辑是通过 getView 方法的 convertView 实现的：当 ListView 的子项滑出屏幕时，
会将整个子项（包含内部的 RecyclerView）放入 ListView 的 “复用池”；当子项重新滑入屏幕时，
ListView 会复用这个 convertView，但不会保留 RecyclerView 内部的状态（比如 RecyclerView 的滑动位置、子项复用池、数据缓存）。
这会导致两个严重问题：
RecyclerView 频繁重建 / 重绑定：每次 ListView 复用包含 RecyclerView 的子项时，
都需要重新为 RecyclerView 设置 Adapter、刷新数据（甚至重新初始化 LayoutManager）—— 相当于每次滑入屏幕，
都要 “重新创建一个小的 RecyclerView”，RecyclerView 自身的高效复用池（RecycledViewPool）完全失效，
性能比单独使用 RecyclerView 差一个量级。
状态丢失：RecyclerView 的滑动位置、子项选中状态等会随 ListView 的复用而丢失（比如滑动 ListView 后再滑回，
内部 RecyclerView 会回到初始滚动位置，之前加载的子项需要重新创建），用户体验极差。


//创建一个LinearLayoutManager线性布局对象，并将其设置到RecyclerView当中
val layoutmanager = LinearLayoutManager(this)
layoutmanager.orientation = LinearLayoutManager.HORIZONTAL
recycler_view.layoutManager = layoutmanager
//创建FruitAdapter实例并将水果数据传入FruitAdapter构造函数中
val adapter = FruitAdapter(fruit_list)
//最后调用setAdapter来完成适配器设置，从而完成RecyclerView与数据的关联
recycler_view.adapter = adapter
————————————————

布局：
Linearlayout:orientation表示布局的方向，gravity表示文字在控件中的对齐方式，layout_gravity
表明控件在布局中的对齐方式。

RelativeLayout:基于边缘和基线对齐
相对于父容器进行定位:layout_alignParentLeft,......
根据兄弟组件进行定位：layout_toLeftof,layout_toRightof,.........

constraintLayout:基于约束关系进行高精度定位
用于控件与父布局 / 其他控件的关联约束，通过 layout_constraint[源位置]_to[目标位置]Of 形式定义：
layout_constraintLeft_toLeftOf：控件左边缘对齐目标左对齐（如父布局左边缘、其他控件左边缘）
layout_constraintLeft_toRightOf：控件左边缘对齐目标右边缘
layout_constraintRight_toLeftOf：控件右边缘对齐目标左边缘
layout_constraintRight_toRightOf：控件右边缘对齐目标右边缘,.......

设置组件和父容器之间的边距：layout_marginLeft,layout_marginRight,.....
设置文字和控件之间的边距：paddingLeft,paddingRight,.......

fragment使用:https://blog.csdn.net/JMW1407/article/details/114069742

fragment的四种生命周期：运行状态，暂停状态，停止状态，销毁状态
运行状态：与fragment关联的activity正在运行
暂停状态：与fragment关联的activity进入暂停状态，所以fragment也一同进入暂停状态，此时fragment
可能被其他的activity覆盖，但仍可能是部分可见。
停止状态：activity进入停止状态或者是fragmentManager调用了remove或replace并且在此之前调用了addtobackstack
方法，此时fragment完全不可见
销毁状态：activity被销毁或者是fragmentManager调用了remove或replace并且在此之前没有调用了addtobackstack
方法

完整生命周期：
onAttach->onCreat->onCreatView->onViewCreated->onActivityCreat->onStart->onResume->onPause->onStop->onDestroyView
->onDestroy->onDetach

动态添加碎片主要分为 5 步：
1. 创建待添加的碎片实例
2. 获取 FragmentManager，在活动中可以直接调用 getSupportFragmentManager()方法得到
3. 开启一个事务，通过调用 beginTransaction()方法开启
4. 向容器内添加或替换碎片，使用 replace() 方法实现，需要传入容器的 id 和待添加的碎片实例。
5. 提交事务，调用 commit()方法来完成
public class FragmentTransactionTest extends Activity {

    @Override
        protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.fragment_transaction_test);

        // 步骤1：获取FragmentManager
        FragmentManager fragmentManager = getFragmentManager();

        // 步骤2：获取FragmentTransaction
        FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();

        // 步骤3：创建需要添加的Fragment ：ExampleFragment
        ExampleFragment fragment = new ExampleFragment();

        // 步骤4：动态添加fragment
        // 即将创建的fragment添加到Activity布局文件中定义的占位符中（FrameLayout）
        fragmentTransaction.add(R.id.about_fragment_container, fragment);
        fragmentTransaction.commit();
    }

    // 继承与Fragment
    public static class ExampleFragment extends Fragment {
        @Override
            public View onCreateView(LayoutInflater inflater, ViewGroup container,
                Bundle savedInstanceState) {

            return inflater.inflate(R.layout.example_fragment, container, false);
            // 将example_fragment.xml作为该Fragment的布局文件
        }
    }
}

Fragment和Activity之间的通信：
1.通过接口进行通信：在fragment中定义接口，然后调用接口，注意activity中要实现对应的接口
2.通过bundel传递数据进行通信：
// 1. 在Activity中创建Fragment并传递数据
MyFragment fragment = new MyFragment();
Bundle bundle = new Bundle();
bundle.putString("key", "Hello from Activity");
fragment.setArguments(bundle);

// 2. 在Fragment中接收数据
@Override
public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    if (getArguments() != null) {
        String data = getArguments().getString("key");
        // 使用数据
    }
}
需要注意的是：activity通过bundle传递的数据对象和fragment实际接收到的数据对象并不是同一个对象，这是因为activity
将数据对象放入Bundle之后，Bundle会将数据对象进行序列化然后发送，fragment从bundle再反序列化出数据对象，这两个数据
对象的内存地址实际上是不一样的

3.通过共享viewModel实现两者通信：
其本质是创建一个viewModel,viewModel中提供了获取livedata的方法以及设置数据的方法，然后activity
和fragment都持有livedata并且观察数据，基于livedata的特性，无论哪一个设置了数据，另一方都能够感知的到
// 1. 创建共享ViewModel
public class SharedViewModel extends ViewModel {
    private final MutableLiveData<String> selectedData = new MutableLiveData<>();

    public void setData(String data) {
        selectedData.setValue(data);
    }

    public LiveData<String> getData() {
        return selectedData;
    }
}

// 2. 在Activity中使用
public class MainActivity extends AppCompatActivity {
    private SharedViewModel viewModel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        viewModel = new ViewModelProvider(this).get(SharedViewModel.class);
        viewModel.getData().observe(this, data -> {
            // 接收来自Fragment的数据
            Log.d("MainActivity", "Received: " + data);
        });

        // 向Fragment发送数据
        viewModel.setData("Hello from Activity");
    }
}

// 3. 在Fragment中使用
public class MyFragment extends Fragment {
    private SharedViewModel viewModel;

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        // 使用Activity的ViewModelProvider，确保共享同一个ViewModel实例
        viewModel = new ViewModelProvider(requireActivity()).get(SharedViewModel.class);

        // 接收来自Activity的数据
        viewModel.getData().observe(getViewLifecycleOwner(), data -> {
            Log.d("MyFragment", "Received: " + data);
        });

        // 向Activity发送数据
        viewModel.setData("Hello from Fragment");
    }
}

service学习:https://juejin.cn/post/7339636980265566217
service的启动方式有两种：startService和bindService。
通过startService启动之后，service的生命周期和activity之间是没有关系的，即使activity被销毁，service
任然可以存在。

-------------------------------------------------------------------------------
什么是IBinder:
在 Android 开发中，IBinder 是跨进程通信（IPC）的核心接口，也是 Android binder 机制的基础组件。
它定义了进程间通信的基本协议，是不同进程之间传递数据和调用方法的桥梁
1.跨进程通信的媒介：Android 中，每个应用通常运行在独立进程中，进程间内存隔离，无法直接共享数据。
IBinder 提供了一种机制，让一个进程可以通过它与另一个进程的组件（如 Service）建立连接并交互。
2.每个IBinder实例对应一个远程对象（如远程 Service），进程通过持有 IBinder引用，
可以标识并访问该远程对象

bindService的过程：
fun bindService(
    intent: Intent,          // 用于指定目标服务
    connection: ServiceConnection,  // 绑定结果的回调接口
    flags: Int               // 绑定选项（如 BIND_AUTO_CREATE 自动创建服务）
): Boolean
intent：明确要绑定的服务（显式 Intent 为主，如 Intent(this, MyService::class.java)）。
ServiceConnection：关键回调接口，用于接收绑定成功 / 失败的通知，并获取服务返回的 IBinder 对象。
flags：常用 Context.BIND_AUTO_CREATE（若服务未启动，则绑定的同时自动创建服务）。

组件（如 Activity）调用 bindService()，系统通过 Intent 定位到目标服务，并检查服务是否已创建：
若服务未创建且 flags 包含 BIND_AUTO_CREATE，则先触发服务的 onCreate() 方法。
若服务已创建，则直接进入绑定流程。
class MyService : Service() {
    // 创建Binder子类，定义服务可被调用的方法
    private val binder = object : Binder() {
        fun getService(): MyService = this@MyService  // 暴露服务实例
        fun doSomething() { ... }  // 服务提供的方法
    }

    // 绑定服务时调用，返回Binder对象
    override fun onBind(intent: Intent): IBinder {
        return binder  // 返回自定义Binder
    }
}

系统将服务返回的 IBinder 传递给组件的 ServiceConnection回调：
// 组件中定义ServiceConnection
private val connection = object : ServiceConnection {
    // 绑定成功时调用
    override fun onServiceConnected(className: ComponentName, service: IBinder) {
        // 将IBinder转换为自定义Binder，获取服务引用
        val binder = service as MyService.Binder
        val myService = binder.getService()  // 获取服务实例
        myService.doSomething()  // 调用服务方法
    }

    // 绑定意外断开时调用（如服务崩溃）
    override fun onServiceDisconnected(className: ComponentName) {
        // 处理断开逻辑
    }
}
onServiceConnected：绑定成功后调用，通过 IBinder 可直接与服务交互（核心回调）。
onServiceDisconnected：非主动解绑时（如服务异常终止）调用，通常用于清理资源。

------------------------------------------------------------------------------------


Retrofit学习：https://blog.csdn.net/JMW1407/article/details/114237331
Okhttp请求总结
Step1：创建HttpClient对象，也就是构建一个网络类型的实例，一般会将所有的网络请求使用同一个单例对象。
Step2：构建Request，也就是构建一个具体的网络请求对象，具体的请求url，请求头，请求体等等。
Step3：构建请求Call，也就是将具体的网络请求与执行请求的实体进行绑定，形成一个具体的正式的可执行实体。
Step4：后面就进行网络请求了，然后处理网络请求的数据了。

http请求的类型：
1.GET:向服务器请求指定的资源
2.POST：根据报文对服务器指定资源进行处理
3.PUT：向服务器提交数据，更新资源
4.DELETE：请求服务器删除指定资源

网络请求本质还是由Okhttp完成的，retrofit只是完成网络请求接口的封装。
App应用程序通过Retrofit请求网络，实际上是使用Retrofit接口层封装请求参数、Header、Url 等信息，
之后由 OkHttp 完成后续的请求操作。在服务端返回数据之后，OkHttp 将原始的结果交给 Retrofit，
Retrofit根据用户的需求对结果进行解析。

retrofit请求流程：
1、引用，在gradle文件中引用retrofit
2、定义接口，Retrofit要求定义一个网络请求的接口，接口函数里要定义url路径、请求参数、返回类型。
3、依次获得Retrofit对象、接口实例对象、网络工作对象
首先，需要新建一个retrofit对象。
然后，根据上一步的接口，实现一个retrofit加工过的接口对象。
最后，调用接口函数，得到一个可以执行网络访问的网络工作对象。
4、访问网络数据，用上一步获取的worker对象，执行网络请求，在回调函数里，取得我们需要的BizEntity数据对象。网络访问结束。
Retrofit并不做网络请求，只是生成一个能做网络请求的对象。
Retrofit的作用是按照接口去定制Call网络工作对象


Rxjava学习：
https://blog.csdn.net/ezconn/article/details/133844948
RxJava 的观察者模式
RxJava 有四个基本概念：Observable(可观察者，即被观察者)、 Observer(观察者)、 subscribe(订阅)、事件。
Observable 和 Observer 通过 subscribe() 方法实现订阅关系，从而 Observable 可以在需要的时候发出
事件来通知 Observer。与传统观察者模式不同， RxJava 的事件回调方法除了普通事件 onNext()
（相当于 onClick() / onEvent()）之外，还定义了两个特殊的事件：onCompleted() 和 onError()。

onCompleted() : 事件队列完结。RxJava 不仅把每个事件单独处理，还会把它们看做一个队列。RxJava 规定，
当不会再有新的 onNext() 发出时，需要触发 onCompleted()方法作为标志。
onError() : 事件队列异常。在事件处理过程中出异常时，onError() 会被触发，同时队列自动终止，
不允许再有事件发出。在一个正确运行的事件序列中, onCompleted() 和 onError() 有且只有一个，并且
是事件序列中的最后一个。需要注意的是，onCompleted() 和 onError() 二者也是互斥的，即在队列
中调用了其中一个，就不应该再调用另一个。

Rxjava创建操作符：just, 发射指定的单个或多个数据（最多 10 个），最后自动调用 onComplete。
Rxjava转换操作符: 一对一转换：将数据类型 T 转换为 R（如 String → Integer）。


Rxjava框架:这是一种响应式编程的思维，是从起点到终点的一个过程，当前层接收到上一层传来的数据，在当前
层完成需求功能之后将数据再传到下一层,当前层的数据的类型和上一层数据的类型是一致的。
使用过程：
obervable.just()
         =>  //todo
            .subscribeOn(Schedulers.io())  //上一层做一些网络异步请求，需要分配异步线程
                .observeOn(AndroidSchedulers.mainThread()   //回到终点需要订阅安卓主线程
                    .subscribe(new observer<T>) {
                        @override
                            //表示起点和终点订阅关联成功
                            public void Onsubscribe() {

                                             }
                            //拿到上一层给我的响应
                        @override
                            public void onNext(T s) {`

                        }
                           //链条思维发生了异常
                        @override
                            public void onError(T s) {

                        }
                          //整个链条全部结束
                        @override
                            public void onComplete(T s) {

                        }

}

八股问题：
一个activity启动另一个activity覆盖自己的过程中，两个activity的哪些生命周期会被调用，以什么顺序调用?
启动另一个：第一个activity(onPause)->第二个activity(onCreat,onStart,onResume)->第一个activity(onStop)
如果又返回第一个activity,经历的周期：
第二个(onPause)->第一个(onRestart)->第一个(onStart)->第一个(onResum)->第二个(onStop)->第二个(onDestroy)


如何让部分代码运行在一个独立的进程中？进程之间怎么进行通信？
可以通过创建一个继承Service的类，然后在该类中编写代码，同时需要在配置文件中声明service，并且声明
android:process（指定运行的进程名)。
1.我所使用到的进程之间的通信就是通过bundle,将bundle添加到intent中，然后在不同进程的组件间进行简单的数据
传输。
2.通过ibinder进行通信，例如绑定其他进程的service
3.通过socket进行进程间通信 


为什么只能在UI线程操作UI？
因为安卓的UI组件，本质上是非线程安全的，如果允许多线程操作UI的话，需要通过加锁来保证线程安全，
Android限制“只能在 UI 线程操作 UI”，是为了避免多线程并发导致的 UI 状态混乱和性能问题，
通过框架层强制约束保证了 UI 操作的安全性和高效性。

因为规定只能在UI线程中修改UI，那如果想让新启动的线程修改UI的属性值应该怎么办呢？
可以通过handler类来实现，子线程想要修改Activity中的UI组件时，可以创建一个handler对象，如果通过handler
对象来给主线程发信息，给主线程发的信息都会存入到messagequeue中，然后looper按照先入先出的顺序取出
message，并且根据message中的what属性分发给对应的handler进行处理

子线程更新主线程的UI的完整过程：
public class MainActivity extends AppCompatActivity {

    private static final String TAG = "UIUpdateDemo";
    private static final int MSG_UPDATE_UI = 1;

    private TextView statusTextView;
    private Button updateButton;

    // 1. 在主线程中创建一个Handler实例
    // 这个Handler将负责处理来自子线程的消息
    private final Handler uiHandler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(Message msg) {
            // 4. 这个方法是在主线程执行的！
            Log.d(TAG, "handleMessage: UI thread is handling the message. Thread: " + Thread.currentThread().getName());

            switch (msg.what) {
                case MSG_UPDATE_UI:
                    // 在这里安全地修改UI
                    String newText = (String) msg.obj; // 从消息中取出数据
                    statusTextView.setText(newText);
                    updateButton.setEnabled(true); // 恢复按钮可点击状态
                    break;
                default:
                    super.handleMessage(msg);
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        statusTextView = findViewById(R.id.status_text_view);
        updateButton = findViewById(R.id.update_button);

        updateButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 点击按钮后，禁用按钮防止重复点击
                updateButton.setEnabled(false);
                statusTextView.setText("正在更新...");

                // 2. 启动一个子线程来执行耗时任务
                new Thread(new BackgroundTask()).start();
            }
        });
    }

    // 定义一个Runnable，代表子线程要执行的任务
    private class BackgroundTask implements Runnable {
        @Override
        public void run() {
            // 3. 这部分代码在子线程中执行
            Log.d(TAG, "run: Background thread is starting the task. Thread: " + Thread.currentThread().getName());

            // 模拟一个耗时操作，比如网络请求或数据库查询
            try {
                Thread.sleep(3000); // 暂停3秒
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            // 任务完成，准备更新UI
            String result = "UI更新成功！";

            // 检查当前线程是否是主线程（为了演示）
            if (Looper.myLooper() == Looper.getMainLooper()) {
                Log.d(TAG, "run: This is the main thread. (This should NOT happen)");
            } else {
                Log.d(TAG, "run: This is a background thread. Ready to send message to UI thread.");
            }
            
            // 错误的做法：直接在子线程修改UI，会导致程序崩溃！
            // statusTextView.setText(result); 

            // 正确的做法：通过Handler向主线程发送一个消息
            Message message = Message.obtain();
            message.what = MSG_UPDATE_UI; // 设置消息类型
            message.obj = result;         // 携带数据
            uiHandler.sendMessage(message); // 将消息放入主线程的消息队列

            Log.d(TAG, "run: Background thread has sent the message. Exiting.");
        }
    }
}



怎么创建一个持续接收工作的后台线程，如何使用线程池？
1.持续接收工作的线程需要一个任务队列和循环机制，不断从队列中获取并执行任务。
核心思路是：线程启动后进入循环，通过阻塞队列阻塞队列 ** 等待新任务，有任务时执行，执行完继续等待。
2.使用线程池

什么是Livedata:
livedata是一个可观察的数据持有类，当数据发生变化时可以通知观察者。
1.数据观察与通知：LiveData 持有某种类型的数据（如字符串、对象、集合等），当数据发生变化时，会主动
通知所有注册的观察者。这使得UI能够实时反映数据状态，无需手动刷新。
例如：当网络请求返回新数据时，LiveData 会自动通知 Activity 更新界面。
2.生命周期感知：LiveData能感知观察者（如 Activity）的生命周期状态（活跃 / 销毁等），只在组件处于活跃
状态（STARTED 或 RESUMED）时发送通知，避免在组件销毁后执行无用的 UI 操作，从而防止内存泄漏。
例如：当 Activity 被销毁后，LiveData 会自动移除对它的引用，不会导致因持有 Activity
引用而引发的内存泄漏。
3.配置变更的适应：当屏幕旋转等配置变更导致组件（如 Activity）重建时，LiveData会保留最新的数据，
新创建的组件实例可以立即获取到最新数据，无需重新请求或计算。
例如：旋转屏幕后，新的 Activity 实例能直接从 LiveData 中获取之前的数据，避免重复加载。
4.简化线程管理：LiveData 默认在主线程（UI 线程）通知观察者，开发者无需手动处理线程切换，
确保 UI 操作始终在主线程执行，避免线程安全问题。

mvp架构和mvvm架构：https://www.cnblogs.com/huhewei/p/14111884.html
viewModel是怎么实现view层和model层的双向绑定的：
1.viewModel持有model的引用，采用发布-订阅模式监听model层的变化，model层为被观察者，当model层变化时
通知viewmodel，viewModel会更新自己暴露给view的可绑定的属性
2.viewModel对于view的绑定：
在 MVVM 架构中，通常由ViewModel持有LiveData对象，Repository 层获取数据后更新 LiveData，
UI 层（Activity/Fragment）观察 LiveData 并在数据变化时更新界面，形成「数据驱动 UI」的模式，
大幅简化了数据与 UI 的交互逻辑。
3.view的事件转发：View 的用户操作（如输入框文本变化、按钮点击）会通过事件绑定
（如onTextChanged、onClick）关联到 ViewModel 的方法。ViewModel 在这些方法中处理业务逻辑，
并调用 Model 的 setter 方法更新数据，完成 “View → Model” 的同步



安卓开发内存泄露的本质是：本该被回收的对象，意外地被一条“存活” 的引用链所持有，也就是长生命
周期的对象持有短生命周期对象的引用
例如：
一个Activity已经销毁（用户关闭页面），但被一个静态变量或后台线程持有引用，导致它无法被回收。
一个大图片对象已经不在 UI 上显示，但仍被某个集合（如List）引用，导致内存占用无法释放。
举例：
1.静态变量持有 Activity/Fragment引用：静态变量的生命周期与应用进程一致，若持有销毁后的Activity引用，
会导致其无法回收：
public class MainActivity extends AppCompatActivity {
    // 错误示例：静态变量持有Activity引用
    private static MainActivity sInstance;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        sInstance = this; // 危险！Activity销毁后仍被持有
    }
}
2.非静态内部类（如 Handler、Thread、Runnable）会隐式持有外部类（如 Activity）的引用，
若内部类对象生命周期长于外部类，会导致外部类泄漏。
class MainActivity : AppCompatActivity() {
    // 非静态Handler持有MainActivity引用
    private val mHandler = object : Handler(Looper.getMainLooper()) {
        override fun handleMessage(msg: Message) {
            // 处理消息
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        // 发送延迟消息（10分钟后执行）
        mHandler.postDelayed( 10 * 60 * 1000)
    }
}


一个组件从创建到显示到屏幕上需要经过什么样的过程？
流程的起点是ViewRootImpl的performTraversals()方法，它会依次调用measure()、layout()、draw()，
完成整个绘制流程。
其中，测量和布局是确定组件位置和大小的关键环节，缺一不可
测量：确定组件的大小 ,递归执行：如果组件是容器（如ViewGroup），会先测量自己，
再遍历所有子组件，调用子组件的measure()方法，完成整个布局树的测量
布局：确定组件的位置
绘制：根据测量和布局的结果，将View的内容（文本、图像、背景等）绘制到画布（Canvas）上
顺序性：必须先完成测量，才能进行布局；必须先完成布局，才能进行绘制（依赖前序阶段的结果）。
递归性：ViewGroup的测量、布局、绘制都会触发子View的对应流程（从顶层DecorView开始，
逐层递归到所有子View）。

constraintLayout相比传统其他的布局的优势是什么？
1.布局灵活性  2.层级优化  3.开发效率

布局灵活：定位灵活，可以实现精准定位。
层级优化：传统布局实现UI时，往往需要多层的嵌套，LinearLayout嵌套RelativeLayout或者多层linearLayout
嵌套等，嵌套层级过深的话会导致测量和布局环节的耗时增加，并且可能会导致过度绘制。约束布局的优势在于
支持平级布局，即所有的子视图直接放在ConstraintLayout下，不需要嵌套。

application的oncreate和activity的oncreate有什么区别：
1.application的是整个应用生命周期只执行一次，而activity的是每次创建实例时都会调用，一个应用中
可能会执行多次
2.application的是用来初始化全局资源的，而activity的是用来初始化UI组件，设置事件监听等
3.application的生命周期和整个应用是一致的，而activity的生命周期只是从页面创建到销毁

点击事件结束后是怎么被销毁的？
点击事件本身（onClick()方法的执行）是瞬时的，执行完毕后就结束了；
而承载事件的OnClickListener对象的 "销毁"，则依赖于引用链的释放：当没有任何对象持有它的引用时，会被 GC 回收。
避免内存泄漏的关键是：确保事件监听对象的生命周期不超过其宿主组件（如 Activity）的生命周期。

如何提高点击按钮打开某个页面的速度？
1.优化点击事件的响应速度：点击按钮之后会触发回调，避免在点击回调中执行太耗时的操作，将网络请求等耗时操作转移到子线程中
去进行。可以使用java的线程池去进行异步操作
2.优化新页面的初始化的速度：新页面的初始化是在oncreat函数中进行，可以通过异步初始化进行改善或者
通过对象复用，页面异步初始化方面，除了将耗时的数据库查询、数据处理等操作放在子线程进行，还可以使用延迟初始化的策略，
对于一些非首屏必需的组件或功能，在页面显示后再进行初始化。比如，使用 postDelayed 方法延迟加载底部的广告栏、复杂的动画效果等。
3.页面启动时setContentView加载布局会比较耗时，可以使用constraintLayout来替代线性布局和相对布局，从而减少布局的嵌套层数

触摸事件的传递：
主要涉及三个层级：
1.Activity:dispatchTouchEvent(MotionEvent ev)，事件传递的起点，负责将事件分发给窗口window.
2.ViewGroup:dispatchTouchEvent(MotionEvent ev),onInterceptTouchEvent(MotionEvent ev)分发事件，
判断是否拦截事件
3.View:dispatchTouchEvent(MotionEvent ev)，onTouchEvent(MotionEvent ev)分发事件，处理事件

dispatchTouchEvent(ev)：
返回true：事件被当前 View 消费（不再传递）；
返回false：事件未被消费，向上传递给父 View 的onTouchEvent处理。

onInterceptTouchEvent(ev)（仅ViewGroup）：
返回true：拦截事件（事件不再传给子 View，改由自身onTouchEvent处理）；
返回false：不拦截（事件继续传给子 View）。

onTouchEvent(ev)：
返回true：当前 View 处理了事件（消费）；
返回false：当前 View 不处理，事件向上传递给父 View 的onTouchEvent。

责任链模式：事件传递就像一条责任链。dispatchTouchEvent是链条的起点和调度员。它先问onInterceptTouchEvent是否要拦截，
如果不拦截，就把事件传给子 View 的dispatchTouchEvent，如此递归。如果没人拦截，最终到达叶子节点View，
由它的onTouchEvent处理。如果处理了（返回true），链条结束；如果没处理（返回false），事件会沿着链条回溯，让父节点有机会处理。

ANR是什么，ANR产生的根本原因是什么？
在安卓开发中，ANR（Application Not Responding，应用无响应） 是指应用在特定时间内无法响应用户操作或系统事件，
导致系统弹出 “应用未响应” 对话框，提示用户选择 “等待” 或 “关闭应用”。ANR 会严重影响用户体验，
是开发中需要重点规避的问题

一、ANR 的触发条件
安卓系统通过监控应用的关键线程（主要是 主线程 / UI 线程）的响应时间来判断是否发生 ANR。不同场景下的触发阈值不同：
用户交互触发（输入事件）
当用户进行点击、触摸、按键等操作时，主线程需在 5 秒内 处理完事件并反馈。若超过 5 秒未响应，触发 ANR。
BroadcastReceiver 触发
静态或动态注册的广播接收器，需在 10 秒内 完成 onReceive() 方法的执行。若超时，触发 ANR。
Service 触发
Service 处于前台时（startForeground），需在 20 秒内 完成 onCreate()、onStartCommand() 等生命周期方法；
后台 Service 需在 200 秒内 完成上述操作（不同系统版本可能有差异）。超时则触发 ANR。
ContentProvider 触发
内容提供者的 onCreate() 方法需在 10 秒内 完成初始化，超时会导致依赖它的进程 ANR。

-----------------------------------------------------------
实习Rxjava+retrofit实例应用:
   Api.getInstance()
       获取Api单例对象，项目把retrofit实例的创建，基础配置(baseUrl, 拦截器)等
       都封装在Api类里面了，通过单例模式确保全局统一的网络请求配置。

       .getOutStockRetrofit()
         从Api单例中获取专门用于出库相关的retrofit实例，不同的retrofit实例可以配置不同的baseUrl,
       等。

       .creat(IsowService.class)
       利用retrofit的creat方法，根据定义的IsowService接口动态生成对于的网络请求服务实例，这样
       就可以调用接口中定义的具体的请求方法.

       .getContainerSowingTaskSummary(.....)
       如上所说调用接口中具体的方法，发起一个网络请求，这个请求会返回一个observable对象，后续
       通过Rxjava的操作符来处理请求的订阅和线程切换等。

       .compose(activity.bindUntilEvent(ActivityEvent.Destory))
       compose:可以将多个操作符的组合（如线程切换、数据转换、生命周期管理等）封装成一个 Transformer
       对象，通过 compose()应用到 Observable上。这样可以避免重复编写相同的逻辑，提高代码复用性。
       bindUntilEvent 通常是 RxLifecycle 库（如 com.trello.rxlifecycle2）提供的方法，
       主要作用是 将 RxJava 数据流的生命周期与 Android 组件（如 Activity、Fragment）的生命周期绑定
       ，从而自动管理订阅关系，避免内存泄漏（即当activity被销毁之后不再持有activity的引用了，这样
       可以使得activity被回收）。
       这是 RxLifecycle 库提供的生命周期管理功能，将 RxJava 数据流与 Activity 生命周期绑定。
       当Activity触发 Destroy事件时，会自动取消订阅，避免内存泄漏。

       .observeOn(AndroidSchedulers.mainThread())
       切换为安卓主线程，因为安卓规定只有在主线程中才可以更新UI

       .subscribe(..............................) {
          @override
              public void OnFailed() {

              public void OnSucess() {

          }
   }
-----------------------------------------------------------------------------------
实习react-native部分:

usePageEvent：页面生命周期事件绑定
绑定页面生命周期：监听框架定义的页面事件（如 ON_PAGE_LOAD，类似 React 的 useEffect，但更贴近业务场景的
“页面加载完成” 时机）。触发逻辑编排：在事件回调中，执行初始化逻辑（如配置页面状态、设置 Header、
处理参数、加载数据）。

//路由跳转/页面初始化完成后，usePageEvent(ON_PAGE_LOAD)的回调执行
本质：把 “状态→显示→交互” 的规则提前配置好，后续页面状态变化时（比如请求失败触发 PageStatus.ERROR），
框架会自动套用这些规则，减少重复代码。
usePageEvent(ON_PAGE_LOAD, () => {
  setPageStatusConfig({  // 配置不同页面状态的行为（重点看！）
    [PageStatus.ERROR]: {
      onPressRefreshButton: () => {
        service.loadData(); // 错误状态下，点击重试按钮的逻辑
      },
      title: errorMsg       // 错误状态下显示的文案
    }
  });

  setHeader({  // 设置页面头部右侧内容（比如导航栏）
    right: <NavigatorRight showMis={true} />
  });

  // 处理 URL 参数，初始化 service 的属性
  if (pageQuery.containerCode && pageQuery.sowingTaskDetailId) {
    service.containerCode = pageQuery.containerCode;
    service.sowingTaskDetailId = pageQuery.sowingTaskDetailId;
    service.loadData(); // 加载数据
  } else {
    toast('数据异常'); // 参数缺失，提示错误
  }
});

 ---------------------------------------------------------------------------------------

 usePageStatus：页面状态管理与切换
管理页面状态机：维护页面的状态（如 CONTENT（内容正常）、ERROR（数据错误）、LOADING（加载中）等）。
驱动 UI 渲染：根据 pageStatus 的值，决定页面显示 “内容”“错误提示”“加载中” 等不同 UI。

const setPageStatus=usePageStatus();
useEffect(() => {
  setPageStatus(pageStatus); // 监听pageStatus 变化，同步到状态管理
}, [pageStatus, setPageStatus]);

if (pageStatus !== PageStatus.CONTENT || !detailData) {
  // 根据状态，决定是否渲染某些内容（比如loading、错误页）
}

 ---------------------------------------------------------------------------------------

 依赖注入+useService：通过useService可以直接获得注入的服务实例，然后可以调用服务的相关方法
 如果手动创建 const dataService = new DataService()，组件每次重新渲染都会创建新实例，
 导致资源重复创建（比如多个定时器同时运行）；且组件销毁时，需要手动调用 dataService.destroy()，容易遗漏导致内存泄漏。
 // 2. 定义带清理逻辑的服务
class DataService {
  // 模拟网络请求的订阅
  subscription = null;

  fetchData() {
    // 模拟一个长期存在的订阅（如WebSocket、定时器）
    this.subscription = setInterval(() => {
      console.log('定期刷新数据');
    }, 1000);
  }

  // 清理方法：释放资源
  destroy() {
    if (this.subscription) {
      clearInterval(this.subscription); // 取消定时器
      console.log('DataService已清理');
    }
  }
}
// 3. 在组件中使用useService
import { useEffect } from 'react';

function DataComponent() {
  // 获取服务实例
  const dataService = useService(DataService);

  useEffect(() => {
    // 调用服务方法
    dataService.fetchData();
  }, [dataService]);

  return <div>数据组件</div>;
}

 ---------------------------------------------------------------------------------------

通过useState + useEffect实现复杂状态管理
useState和useEffect是 React 的核心 Hook，组合使用可处理组件中的多状态联动、异步状态更新、依赖外部数据
的状态变化等复杂场景。
useState：管理组件内部的状态（如数据、加载状态、错误信息），返回 “状态值” 和 “更新函数”。
useEffect：处理 “副作用”（如网络请求、事件监听、DOM 操作），可依赖特定状态触发执行，
模拟生命周期（如componentDidMount、componentDidUpdate）

import { useState, useEffect } from 'react';
function ProductList() {
  // 1. 定义多个关联状态
  const [products, setProducts] = useState([]); // 商品列表数据
  const [loading, setLoading] = useState(false); // 加载状态
  const [error, setError] = useState(null); // 错误信息
  const [page, setPage] = useState(1); // 分页页码

  // 2. 副作用：依赖page变化时加载数据
  useEffect(() => {
    // 加载开始：更新状态
    setLoading(true);
    setError(null);

    // 异步请求：获取商品列表
    const fetchProducts = async () => {
      try {
        const response = await fetch(`/api/products?page=${page}&size=10`);
        if (!response.ok) throw new Error('请求失败');
        const data = await response.json();

        // 分页逻辑：第一页直接替换，后续页追加
        setProducts(prev => page === 1 ? data.list : [...prev, ...data.list]);
      } catch (err) {
        setError(err.message); // 保存错误信息
      } finally {
        setLoading(false); // 无论成功失败，结束加载
      }
    };

    fetchProducts();
  }, [page]); // 仅当page变化时重新执行（避免无限循环）

  // 3. 处理用户交互：加载更多（更新page状态，触发useEffect）
  const handleLoadMore = () => {
    if (!loading) setPage(prev => prev + 1);
  };

 ---------------------------------------------------------------------------------------


 */
