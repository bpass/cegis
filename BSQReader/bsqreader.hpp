template <class T>
Pixel<T> BSQReader<T>::getPixel(const Point& p, const int layer)
{

	T value = 0;

	//check for idiotic dimensions
	if((p.x() < 0) || (p.y() < 0) || (layer < 0))
		throw DimException(__LINE__, __FILE__);

	long width = m_imageInfo.width();
	long height = m_imageInfo.height();

	//pixel offset = (image_width*row + col) + layer_offset
	long offset = (width*p.y() + p.x()) + (width*height*layer);
	
	//seek offset*sizeof(data type) from beginning of file.
	m_fileStream.seekg(offset*sizeof(T), ios_base::beg);
	m_fileStream.read((char*)&value, sizeof(T));

	Pixel<T> pixel(p, value);

	return(pixel);
}