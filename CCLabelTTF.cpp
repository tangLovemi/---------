
// Helper
bool CCLabelTTF::updateTexture()
{
	//if(this->isFlipY())
	//	this->setFlipY(false);
    CCTexture2D *tex;
    tex = new CCTexture2D();
    
    if (!tex)
        return false;

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
        ccFontDefinition texDef = _prepareTextDefinition(true);
        tex->initWithString( m_string.c_str(), &texDef );
    
    #else
    
        tex->initWithString( m_string.c_str(),
                            m_pFontName->c_str(),
                            m_fFontSize * CC_CONTENT_SCALE_FACTOR(),
                            CC_SIZE_POINTS_TO_PIXELS(m_tDimensions),
                            m_hAlignment,
                            m_vAlignment);
    
    #endif
    
    // set the texture
    this->setTexture(tex);
    // release it
    tex->release();
    
    // set the size in the sprite
    CCRect rect =CCRectZero;
    rect.size   = m_pobTexture->getContentSize();
    this->setTextureRect(rect);

	/***************************给文字描黑边begin*********************************/

	CCPoint prePos = this->getPosition();
	ccColor3B color1 = this->getColor();
    float strokeValue = 2;
	CCSize textureSize = this->getContentSize();
    textureSize.width += 2 * strokeValue;
    textureSize.height += 2 * strokeValue;
     
    /* 监测OpenGl的错误状态 */
    glGetError();
     
    /* 创建一张纹理画布 */
    CCRenderTexture *rt = CCRenderTexture::create(textureSize.width, textureSize.height);
    if(!rt)
    {
        CCLog("create render texture failed !!!!");
        //addChild(label);
        return 0;
    }
     
    /* 设置描边的颜色 */
    this->setColor(ccBLACK);
     
    /* 
     *拿到源文字的混色机制，存储以备恢复，并设置新的目标混色机制
     *混色机制设为：源颜色透明度（影响亮度）和目标颜色（影响颜色）
     */
    ccBlendFunc originalBlend = this->getBlendFunc();
    ccBlendFunc func = { GL_SRC_ALPHA, GL_ONE};
    this->setBlendFunc(func);
     CCPoint anchPoint = this->getAnchorPoint();
	 
    /* 这是自定义的一些调整，倾斜了一点 */
    this->setAnchorPoint(CCPoint(0.5, 0.5));
    //label->setRotationX(15);
     
    /* 张开画布，开始绘画 */
    rt->begin();
	this->setFlipY(true);
    for(int i = 0; i < 360; i += 5)//每变化5度绘制一张
    {
        float r = CC_DEGREES_TO_RADIANS(i); //度数格式的转换
        this->setPosition(CCPoint(textureSize.width * 0.5f + sin(r) * strokeValue,textureSize.height * 0.5f + cos(r) * strokeValue));
 
        /* CCRenderTexture的用法，在begin和end之间visit的纹理，都会画在CCRenderTexture里面 */
        this->visit();//画了一次该label
    }
 
    /* 恢复原始的label并绘制在最上层 */
    this->setColor(color1);
    this->setBlendFunc(originalBlend);
    this->setPosition(CCPoint(textureSize.width * 0.5f, textureSize.height * 0.5f));
    this->visit();
	this->setFlipY(false);
    /* 在画布上绘制结束，此时会生成一张纹理 */
    rt->end();
     
    /* 取出生成的纹理，添加抗锯齿打磨，并返回 */
    CCTexture2D *texture = rt->getSprite()->getTexture();
    texture->setAntiAliasTexParameters();// setAliasTexParameters();
	this->setTexture(texture);
	// release it
	rt->release();

	//set the size in the sprite
		rect =CCRectZero;
	rect.size   = m_pobTexture->getContentSize();

	//this->setFlipY(true);
	//this->setFlipX(true);
	this->setTextureRect(rect);
	//if(!this->isFlipY())
	//	this->setFlipY(true);
	this->setAnchorPoint(anchPoint);
	this->setColor(color1);
	this->setPosition(prePos);

	/***************************给文字描黑边end*********************************/
    //ok
    return true;
}
