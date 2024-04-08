import praw
import os
 
# 创建Reddit实例并登录
reddit = praw.Reddit(
    user_agent="top_showerthoughts_submissions_crawler",
    client_id="<CLIENT_ID>",
    client_secret="<CLIENT_SECRET>",
    username="<USERNAME>",
    password="<PASSWORD>"
)
 
# 定义要抓取的subreddit
subreddit = reddit.subreddit('showerthoughts')
 
# 设置图片保存路径
image_folder = 'images'
if not os.path.exists(image_folder):
    os.makedirs(image_folder)
 
# 获取前10个置顶帖子
top_submissions = subreddit.top(limit=10)
 
# 抓取并保存图片
for idx, submission in enumerate(top_submissions):
    # 检查帖子内容是否为图片
    if submission.url.endswith(('.png', '.jpg', '.jpeg', '.gif', '.png')):
        image_path = os.path.join(image_folder, f"{idx}_{submission.title}.png")
        submission.save(image_path)
        print(f"Image saved: {image_path}")
    else:
        print(f"Non-image post: {submission.title}")
