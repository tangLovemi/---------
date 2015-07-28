
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

	/***************************��������ڱ�begin*********************************/

	CCPoint prePos = this->getPosition();
	ccColor3B color1 = this->getColor();
    float strokeValue = 2;
	CCSize textureSize = this->getContentSize();
    textureSize.width += 2 * strokeValue;
    textureSize.height += 2 * strokeValue;
     
    /* ���OpenGl�Ĵ���״̬ */
    glGetError();
     
    /* ����һ�������� */
    CCRenderTexture *rt = CCRenderTexture::create(textureSize.width, textureSize.height);
    if(!rt)
    {
        CCLog("create render texture failed !!!!");
        //addChild(label);
        return 0;
    }
     
    /* ������ߵ���ɫ */
    this->setColor(ccBLACK);
     
    /* 
     *�õ�Դ���ֵĻ�ɫ���ƣ��洢�Ա��ָ����������µ�Ŀ���ɫ����
     *��ɫ������Ϊ��Դ��ɫ͸���ȣ�Ӱ�����ȣ���Ŀ����ɫ��Ӱ����ɫ��
     */
    ccBlendFunc originalBlend = this->getBlendFunc();
    ccBlendFunc func = { GL_SRC_ALPHA, GL_ONE};
    this->setBlendFunc(func);
     CCPoint anchPoint = this->getAnchorPoint();
	 
    /* �����Զ����һЩ��������б��һ�� */
    this->setAnchorPoint(CCPoint(0.5, 0.5));
    //label->setRotationX(15);
     
    /* �ſ���������ʼ�滭 */
    rt->begin();
	this->setFlipY(true);
    for(int i = 0; i < 360; i += 5)//ÿ�仯5�Ȼ���һ��
    {
        float r = CC_DEGREES_TO_RADIANS(i); //������ʽ��ת��
        this->setPosition(CCPoint(textureSize.width * 0.5f + sin(r) * strokeValue,textureSize.height * 0.5f + cos(r) * strokeValue));
 
        /* CCRenderTexture���÷�����begin��end֮��visit���������ử��CCRenderTexture���� */
        this->visit();//����һ�θ�label
    }
 
    /* �ָ�ԭʼ��label�����������ϲ� */
    this->setColor(color1);
    this->setBlendFunc(originalBlend);
    this->setPosition(CCPoint(textureSize.width * 0.5f, textureSize.height * 0.5f));
    this->visit();
	this->setFlipY(false);
    /* �ڻ����ϻ��ƽ�������ʱ������һ������ */
    rt->end();
     
    /* ȡ�����ɵ�������ӿ���ݴ�ĥ�������� */
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

	/***************************��������ڱ�end*********************************/
    //ok
    return true;
}
